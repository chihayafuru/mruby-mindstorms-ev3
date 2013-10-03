class EV3
  class Keypad
    def initialize(ev3)
      @ev3 = ev3
    end

    def up?
      ev3.keypad?(0)
    end

    def enter?
      ev3.keypad?(1)
    end

    def down?
      ev3.keypad?(2)
    end

    def right?
      ev3.keypad?(3)
    end

    def left?
      ev3.keypad?(4)
    end

    def back?
      ev3.keypad?(5)
    end
  end

  class Motor
    attr_reader :ports

    def initialize(ev3, ports)
      @ev3 = ev3
      @ports = ports
    end

    def start
      @ev3.start_motor(@ports)
    end

    def stop
      @ev3.stop_motor(@ports)
    end

    def power(val)
      @ev3.set_motor_power(@ports, val)
    end
  end

  def keypad
    @keypad ||= Keypad.new(self)
  end

  def motor ports
    EV3::Motor.new(self, ports)
  end
end
