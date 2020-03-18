

echo "   __________________________________________________________"
date
rodos-clear.sh
echo "templates for new ports"
rodos-lib.sh template

echo "linux-lib"
rodos-lib.sh linux-x86
echo "linuxMC-lib"
rodos-lib.sh linux-makecontext
echo "posix-lib"
rodos-lib.sh on-posix
echo "posix64-lib"
rodos-lib.sh on-posix64
echo "stm32f4-lib"
rodos-lib.sh stm32f4
echo "sf2-lib"
rodos-lib.sh sf2
echo "raspberrypi3"
rodos-lib.sh raspberrypi3
echo "   __________________________________________________________"

