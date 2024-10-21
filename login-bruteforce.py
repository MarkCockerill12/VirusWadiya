import itertools
import string

def custom_hash(value):
    hash_val = 5381
    prime1 = 31
    prime2 = 131
    for i, c in enumerate(value):
        hash_val = ((hash_val * prime1) ^ (ord(c) * prime2)) + (i * 7)
        hash_val &= 0xFFFFFFFF
    return hash_val

def find_backdoor_password(target_hash):
    charset = string.ascii_lowercase[:11]
    
    # Fixed parts of the password
    prefix = '/'
    middle_fixed = '#'
    suffix = '1'
    # seventh_char = 'e'
    # suffix = 'a'
    counter = 0
    # We need to generate 4 characters: 1st, 4th, 6th, and 8th
    for password_parts in itertools.product(charset, repeat=8):
        counter += 1
        password = (
            password_parts[0] +  # 1st character
            prefix +             # 'hi' (2nd and 3rd characters)
            password_parts[1] +
            password_parts[2] +  # 4th character
            middle_fixed +       # '#' (5th character)
            password_parts[3] +  # 6th character
            password_parts[4] +  # 7th character
            password_parts[5] +  # 8th character
            password_parts[6] +  # 9th character
            suffix +
            password_parts[7]
        )
        
        custom_hash_val = custom_hash(password)
        # print(f"{custom_hash_val}")
        if custom_hash_val == target_hash:
            print(f"Counter: {counter}")
            return password
    
    print(f"Counter: {counter}")
    
    return None


# Requirements:
# 9 letters lowercase out of first 13 characters of the alphabet
# 2nd character must be 'h', 3rd character must be 'i'
# 5th character must be '#' (exception), 7th character must be 'e'
# last character must be 'a'
# no numbers


# Backdoor password: e/hi#gdae1k
# Execution time: 159.457621 seconds

# # Example usage:
import time

target_custom_hash = 3873396811

start_time = time.time()  # Start measuring time
backdoor_password = find_backdoor_password(target_custom_hash)
end_time = time.time()  # End measuring time

execution_time = end_time - start_time  # Calculate execution time

if backdoor_password:
    print(f"Backdoor password:", backdoor_password)
else:
    print("No backdoor password found.")

print(f"Execution time: {execution_time:.6f} seconds")  # Print execution time

# # Example usage:
# target_hash = "1489315438"
# key = 'K'
# encrypted_hash = ''.join(chr(ord(c) ^ ord(key)) for c in target_hash)
# print(encrypted_hash)  # zsrxz~xs
# print(chr(ord('*') ^ ord('a')))