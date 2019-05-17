# System-Monitor

## To setup and compile in Udacity Ubuntu workspace:

1. Clone repository into `/home/workspace/`
```
cd /home/workspace/
git clone https://github.com/oipoistar/CppND-System-Monitor.git
cd CppND-System-Monitor
```
2. Install `ncurses` package
```
sudo apt-get install libncurses5-dev libncursesw5-dev
```
3. Compile and run
```
mkdir build && cd build
cmake ..
make -j32
./SystemMonitor
```

I suggest building it with g++ 9.1. Earlier g++ versions have c++17 support only experiemntaly implemented and the std::filesystem had a lot of bugs.