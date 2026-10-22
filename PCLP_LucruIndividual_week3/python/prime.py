import math

def main():
    start = int(input("start: "))
    end = int(input("end: "))

    for i in range(start, end+1):
        prime = True

        if i < 2:
            prime = False
        
        for d in range(2, int(math.sqrt(i)) + 1):
            if i % d == 0:
                prime = False

        if prime:
            print(i, end=" ")

    print("\nDone")

if __name__ == "__main__":
    main()