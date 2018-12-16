import argparse
import random
import string
import sys


def main(parsed_args):
    if parsed_args.symbol_type == 'ascii':
        symbols = string.ascii_letters + string.digits
    else:
        symbols = list(map(chr, range(sys.maxunicode)))

    with open(parsed_args.output_filename, 'w') as f:
        generated_strings = list()
        while len(generated_strings) < parsed_args.lines_number:
            current_line_length = random.choice(range(parsed_args.max_line_length)) + 1  # from 1 to max-line-length

            current_string = ''.join([
                random.choice(symbols) for _ in range(current_line_length)
            ])

            generated_strings.append(current_string)

        f.write('\n'.join(generated_strings))

        return generated_strings


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Random strings file generator')

    parser.add_argument('--lines-number', type=int)
    parser.add_argument('--max-line-length', type=int)
    parser.add_argument('--output-filename', type=str)
    parser.add_argument('--symbol-type', type=str, choices=['ascii', 'unicode'], default='ascii')

    parsed_args = parser.parse_args()

    main(parsed_args)
