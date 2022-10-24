import datetime
import board
import digitalio

START_MSG = 12345
END_MSG = 56789

gp0 = digitalio.DigitalInOut(board.G0)
gp0.direction = digitalio.Direction.INPUT

curr_value = gp0.value
start = datetime.datetime.now()

bit_counter = 0
byte_value = 0


def is_zero(elapsed):
    return 3 <= elapsed <= 7


def is_one(elapsed):
    return 8 <= elapsed <= 12


def is_init(elapsed):
    return 17 <= elapsed <= 24


res = []


def print_res(res):
    res_print = res
    # print(res)
    values = ["min", "max", "value", "mapped_value", "low_threshold", "high_threshold"]
    if res[0] == START_MSG and res[-1] == END_MSG and len(res) == len(values) + 2:
        print(res)

    if len(res) > len(values) + 2:
        res_print = []

    if res[-1] == END_MSG:
        res_print = []

    return res_print


while True:
    val = gp0.value
    if not curr_value and val:
        # low to high
        start = datetime.datetime.now()

    if curr_value and not val:
        # high to low
        end = datetime.datetime.now()
        elapsed = (end - start).total_seconds() * 1000

        if is_zero(elapsed):
            # bit = 0
            bit_counter += 1
        elif is_one(elapsed):
            # bit = 1
            byte_value += 2 ** bit_counter
            bit_counter += 1

        elif is_init(elapsed):
            # init
            bit_counter = 0

        if bit_counter == 16:
            res.append(byte_value)
            res = print_res(res)
            # print(f"out {res}")

            bit_counter = 0
            byte_value = 0

    curr_value = val
