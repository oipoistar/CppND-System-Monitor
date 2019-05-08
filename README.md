# System-Monitor

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
