import argparse
import random
import string
import sys


class WrongOrderException(Exception):
    pass


def main(parsed_args):
    current_line_number = 0
    previous_line = ''
    with open(parsed_args.input_filename, 'r') as f:
        for current_line in f:
            current_line = current_line.rstrip('\n')
            if previous_line > current_line:
                raise WrongOrderException(f'''
                    Sorting order is WRONG on line = {current_line_number}:
                    [[ {previous_line} ]] > [[ {current_line} ]]
                ''')
            
            previous_line = current_line
            current_line_number += 1


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='File sorting checker')

    parser.add_argument('--input-filename', type=str)

    parsed_args = parser.parse_args()

    try:
        main(parsed_args)
    except WrongOrderException as e:
        print(e)
    else:
        print('Everything is OK!')
