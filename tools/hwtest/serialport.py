"""Minimal 9600 8E1 raw serial port using only the standard library (termios)."""
import os, termios

def open_8e1(path, baud=9600):
    fd = os.open(path, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    attrs = termios.tcgetattr(fd)
    iflag, oflag, cflag, lflag, ispeed, ospeed, cc = attrs
    iflag = termios.INPCK                      # parity checking on, no other input processing
    oflag = 0
    cflag = termios.CS8 | termios.PARENB | termios.CREAD | termios.CLOCAL   # 8 data, even parity, 1 stop
    lflag = 0
    speed = getattr(termios, 'B%d' % baud)
    cc[termios.VMIN] = 0; cc[termios.VTIME] = 0
    termios.tcsetattr(fd, termios.TCSANOW, [iflag, oflag, cflag, lflag, speed, speed, cc])
    termios.tcflush(fd, termios.TCIOFLUSH)
    return fd
