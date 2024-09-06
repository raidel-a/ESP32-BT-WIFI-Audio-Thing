#!/opt/homebrew/Cellar/platformio/6.1.15_2/libexec/bin/python

Import("env")
import os

try:
    from dotenv import load_dotenv
    load_dotenv()
except ImportError:
    def load_dotenv():
        if os.path.exists('.env'):
            with open('.env') as f:
                for line in f:
                    if line.strip() and not line.startswith('#'):
                        key, value = line.strip().split('=', 1)
                        os.environ[key] = value.strip('"').strip("'")
    load_dotenv()

env.Append(CPPDEFINES=[
    ("WIFI_SSID", f'\\"{ os.getenv("WIFI_SSID") }\\"'),
    ("WIFI_PASSWORD", f'\\"{ os.getenv("WIFI_PASSWORD") }\\"'),
])