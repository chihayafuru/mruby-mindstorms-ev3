#include "lms2012.h"
#include <mruby.h>
#include <mruby/data.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


struct ev3_state {
  int lms_ui;
  int lms_pwm;
  unsigned char *keypad;
};

static void ev3_free(mrb_state *mrb, void *ptr) {
  struct ev3_state *state = (struct ev3_state *)(ptr);

  if (state->keypad != MAP_FAILED) {
    munmap(state->keypad, 6);
  }
  if(state->lms_ui > 0) {
    close(state->lms_ui);
  }
  if(state->lms_pwm > 0) {
    close(state->lms_pwm);
  }
  mrb_free(mrb, state);
}

static struct mrb_data_type ev3_type = { "EV3", ev3_free };

static mrb_value
mrb_ev3_initialize(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = mrb_malloc(mrb, sizeof(struct ev3_state));

  DATA_TYPE(self) = &ev3_type;
  DATA_PTR(self) = state;

  state->lms_ui = open(UI_DEVICE_NAME, O_RDWR|O_SYNC);
  state->lms_pwm = open(PWM_DEVICE_NAME, O_WRONLY);

  state->keypad = MAP_FAILED;
  if(state->lms_ui > 0) {
    state->keypad = mmap(NULL, 6, PROT_READ|PROT_WRITE, MAP_SHARED, state->lms_ui, 0);
  }
  return mrb_nil_value();
}

static mrb_value
mrb_ev3_keypad(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = DATA_PTR(self);
  mrb_int key;

  mrb_get_args(mrb, "i", &key);
  if(key >= 0 && key <= 5) {
    return *(state->keypad + key) == 1 ? mrb_true_value() : mrb_false_value();
  }

  return mrb_false_value();
}

static mrb_value
mrb_ev3_set_led(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = DATA_PTR(self);
  mrb_int pattern;

  mrb_get_args(mrb, "i", &pattern);
  if(pattern >= 0 && pattern <= 9) {
    char buf[2] = "0";
    buf[0] = '0'+pattern;
    write(state->lms_ui, buf, 2);
  }
  return mrb_fixnum_value(pattern);
}

static mrb_value
mrb_ev3_start_motor(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = DATA_PTR(self);
  char motor_command[5] = {0,};
  char* portNames;
  int len;
  int cnt;

  mrb_get_args(mrb, "s", &portNames, &len);

  motor_command[1] = 0x00;
  for (cnt = 0 ; cnt < len ; cnt++) {
    switch (portNames[cnt]) {
      case 'A':
        motor_command[1] |= 0x01;
        break;
      case 'B':
        motor_command[1] |= 0x02;
        break;
      case 'C':
        motor_command[1] |= 0x04;
        break;
      case 'D':
        motor_command[1] |= 0x08;
        break;
      default:
        ; /* do nothing */
        break;
    }
  }

  motor_command[0] = opOUTPUT_START;

  write(state->lms_pwm,motor_command,2);

  printf("start motor\n");

  return mrb_nil_value();
}

static mrb_value
mrb_ev3_stop_motor(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = DATA_PTR(self);
  char motor_command[5] = {0,};
  char* portNames;
  int len;
  int cnt;

  mrb_get_args(mrb, "s", &portNames, &len);

  motor_command[1] = 0x00;
  for (cnt = 0 ; cnt < len ; cnt++) {
    switch (portNames[cnt]) {
      case 'A':
        motor_command[1] |= 0x01;
        break;
      case 'B':
        motor_command[1] |= 0x02;
        break;
      case 'C':
        motor_command[1] |= 0x04;
        break;
      case 'D':
        motor_command[1] |= 0x08;
        break;
      default:
        ; /* do nothing */
        break;
    }
  }

  motor_command[0] = opOUTPUT_STOP;

  write(state->lms_pwm,motor_command,2);

  printf("stop motor\n");

  return mrb_nil_value();
}

static mrb_value
mrb_ev3_set_motor_power(mrb_state *mrb, mrb_value self)
{
  struct ev3_state *state = DATA_PTR(self);
  char motor_command[5] = {0,};
  char* portNames;
  int len;
  int power;
  int cnt;

  mrb_get_args(mrb, "si", &portNames, &len, &power);

  for (cnt = 0 ; cnt < len ; cnt++) {
    switch (portNames[cnt]) {
      case 'A':
        motor_command[1] |= 0x01;
        break;
      case 'B':
        motor_command[1] |= 0x02;
        break;
      case 'C':
        motor_command[1] |= 0x04;
        break;
      case 'D':
        motor_command[1] |= 0x08;
        break;
      default:
        ; /* do nothing */
        break;
    }
  }

  if (power < -100)	power = -100;
  if (power > 100)	power = 100;

  motor_command[0] = opOUTPUT_POWER;
  motor_command[2] = power;

  printf("set power\n");

  write(state->lms_pwm,motor_command,3);

  return mrb_nil_value();
}

static mrb_value
mrb_ev3_usleep(mrb_state *mrb, mrb_value self)
{
  mrb_int utime;

  mrb_get_args(mrb, "i", &utime);

  usleep(utime);

  return mrb_nil_value();
}

void
mrb_mruby_mindstorms_ev3_gem_init(mrb_state* mrb) {
  struct RClass *ev3_class = mrb_define_class(mrb, "EV3", mrb->object_class);
  mrb_define_method(mrb, ev3_class, "initialize", mrb_ev3_initialize, ARGS_NONE());
  mrb_define_method(mrb, ev3_class, "keypad?", mrb_ev3_keypad, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ev3_class, "led=", mrb_ev3_set_led, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, ev3_class, "usleep", mrb_ev3_usleep, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ev3_class, "startMotor", mrb_ev3_start_motor, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ev3_class, "stopMotor", mrb_ev3_stop_motor, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ev3_class, "setMotorPower", mrb_ev3_set_motor_power, MRB_ARGS_REQ(2));
}

void
mrb_mruby_mindstorms_ev3_gem_final(mrb_state* mrb) {
  /* free someone */
}
