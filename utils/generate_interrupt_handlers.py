#!/usr/bin/python
import sys

def errcode(n):
    print(f"ISR_ERR({n})")

def noerrcode(n):
    print(f"ISR_NOERR({n})")

def errcode_h(n):
    print(f"ISR_ERR_H({n})")

def noerrcode_h(n):
    print(f"ISR_NOERR_H({n})")

have_errcode = {8, 10, 11, 12, 13, 14}
max_number = 48

def generate_c():
    with open("interrupt_handlers.c") as f:
        print(f.read())

    for i in range(48):
        if i in have_errcode:
            errcode(i)
        else:
            noerrcode(i)
def generate_h():
    with open("interrupt_handlers.h") as f:
        print(f.read())
    for i in range(48):
        if i in have_errcode:
            errcode_h(i)
        else:
            noerrcode_h(i)

if __name__ == '__main__':
    if len(sys.argv) == 1:
        generate_c()
    elif sys.argv[1] == 'c':
        generate_c()
    elif sys.argv[1] == 'h':
        generate_h()
