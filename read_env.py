#!/opt/homebrew/Cellar/platformio/6.1.15_2/libexec/bin/python

Import("env")
from dotenv import load_dotenv
import os

load_dotenv()

env.Append(CPPDEFINES=[
    ("WIFI_SSID", f'\\"{ os.getenv("WIFI_SSID") }\\"'),
    ("WIFI_PASSWORD", f'\\"{ os.getenv("WIFI_PASSWORD") }\\"'),
])
