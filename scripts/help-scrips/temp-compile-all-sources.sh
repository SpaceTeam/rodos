

echo "   __________________________________________________________ linux "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh linux $i
done

echo "   __________________________________________________________ posix "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh posix $i
done

echo "   __________________________________________________________ linux-makecontext "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh linuxMC $i
done

echo "   __________________________________________________________ stm32f4 "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh stm32f4 $i
done

echo "   __________________________________________________________ posix64 "
for i in *.cpp
do
    # echo $i
    rodos-executable.sh posix64 $i
done

