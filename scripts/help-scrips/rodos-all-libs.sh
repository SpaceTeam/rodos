

echo "   __________________________________________________________"
date
rodos-clear.sh
echo "templates for new ports"
rodos-lib.sh template

echo "linux-lib"
rodos-lib.sh linux
echo "linuxMC-lib"
rodos-lib.sh linuxMC
echo "posix-lib"
rodos-lib.sh posix
echo "posix64-lib"
rodos-lib.sh posix64
echo "stm32f4-lib"
rodos-lib.sh stm32f4
echo "raspberrypi3"
rodos-lib.sh raspberrypi3
echo "   __________________________________________________________"

