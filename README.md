# CppND-System-Monitor

Starter code for System Monitor Project in the C++ OOP course. Most of the code is provided for you here. Follow along with the classroom lesson to fill out the `ProcessParser` and `Process` classes to complete your project!

## To setup and compile in Udacity Ubuntu workspace:

1. Clone repository into `/home/workspace/`
```
cd /home/workspace/
git clone https://github.com/oipoistar/CppND-System-Monitor.git
cd CppND-System-Monitor
```
2. Install `ncurses` and `boost` package
```
sudo apt-get install libncurses5-dev libncursesw5-dev libboost-all-dev
```
3. Compile and run
```
mkdir build && cd build
cmake ..
make -j32
./SystemMonitor
```
