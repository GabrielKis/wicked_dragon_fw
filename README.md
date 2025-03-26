# How to setup

Install dependencies for zephyr according to the guide: https://docs.zephyrproject.org/latest/develop/getting_started/index.html

Create and dir with this repo inside and create an Python venv on it

```
mkdir -p ~/zephyr_workspace
cd ~/zephyr_workspace
sudo apt install python3-venv
python3 -m venv ~/zephyr_workspace/.venv
source ~/zephyrproject/.venv/bin/activate
pip install -r wicked_dragon_fw/requirements.txt
```


## To build and flash
Go to the repository folder and execute:

```
west build -p
west flash
```

To use the serial monitor provided by Espressif:
```
west espressif monitor
```


