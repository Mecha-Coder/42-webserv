#!/usr/bin/env python3

import time

def hang():
    while True:
        print("Loop iteration")
        time.sleep(1)

hang()
