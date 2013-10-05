ev3 = EV3.new

motorA = ev3.motor("A")
motorB = ev3.motor("B")

motorA.start
motorB.start

motorA.power(20)
motorB.power(-20)

EV3.usleep 3000000

motorA.power(0)
motorB.power(0)

EV3.usleep 500000

motorA.power(-20)
motorB.power(20)

EV3.usleep 3000000

motorA.stop
motorB.stop
