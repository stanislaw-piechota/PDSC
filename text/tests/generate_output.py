import os

tests = [file for file in os.listdir(os.getcwd()) if file.startswith('in')]

for test in tests:
    test_no = int(test[2:-4])
    print(f"Generating output for test nr {test_no}")
    with open(f'{os.getcwd()}/{test}') as file:
        text = file.read().split('\n')

    with open(f'{os.getcwd()}/out{test_no}.txt', 'w') as file:
        text_to_write = '\n'.join([' '.join(line.split(' ')[::-1]) for line in text[::-1]])
        if text_to_write[-1] != '\n':
            text_to_write += '\n'
        file.write(text_to_write)