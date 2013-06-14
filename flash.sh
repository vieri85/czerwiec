#!\usr\bin\env bash
var_path=`pwd`
echo $var_path
echo "dziwka"
cd $(dirname $0)
var_path=`pwd`
echo $var_path
create_gdb_command() {
    echo "target remote localhost:3333" >> gdbcmd
    echo "monitor reset halt" >> gdbcmd
    echo "load" >> gdbcmd
    echo "monitor reset halt" >> gdbcmd
}
echo "Waiting for target connection..."
create_gdb_command
var_path=`pwd`
echo $var_path
arm-none-eabi-gdb --quiet --batch -x gdbcmd ./01_binaries/micro/main.elf
rm -f gdbcmd

echo "Done."
read