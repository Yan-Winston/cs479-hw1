import matplotlib.pyplot as plt



if __name__ == "__main__":
    try:
        x = input("Enter File Name: ")
        with open(x, "r") as f:

            plt.figure(figsize = (10,6))
            
            if "A" in list(x):
                plt.xlim([-1, 6])
                plt.ylim([-1, 6])
            else:
                plt.xlim([-18, 36])
                plt.ylim([-18, 36])
                

            c = 0
            
            for line in f:
                line = line.strip().split(",")
                if c%10 == 0:
                    if line[2] == "1":
                        plt.plot(float(line[0]), float(line[1]), "go", alpha = 0.1, markersize = 0.5)
                    else:
                        plt.plot(float(line[0]), float(line[1]), "ro", alpha = 0.1, markersize = 0.5)
                c += 1
        
        plt.show()
        print("Done.\n")

    except IOError:
        print("Data file not found.\n")