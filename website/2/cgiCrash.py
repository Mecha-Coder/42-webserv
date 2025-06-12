#!/usr/bin/env python3

def crash():
    print("Crashing now...")
    result = 1 / 0  # ZeroDivisionError
    print(result)

crash()
