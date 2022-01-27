def HanoiTower(n=3):
    stack1 = list(range(1, n+1))
    stack1.reverse()
    stack2 = []
    stack3 = []

    if n % 2 == 0:
        for step in range(1, 2**n):

            if step % 3 == 1:
                if len(stack1) == 0:
                    stack1.append(stack2.pop())
                    print('B --> A')
                elif len(stack2) == 0:
                    stack2.append(stack1.pop())
                    print('A --> B')
                else:
                    if stack1[-1] > stack2[-1]:
                        stack1.append(stack2.pop())
                        print('B --> A')
                    else:
                        stack2.append(stack1.pop())
                        print('A --> B')

            elif step % 3 == 2:
                if len(stack1) == 0:
                    stack1.append(stack3.pop())
                    print('C --> A')
                elif len(stack3) == 0:
                    stack3.append(stack1.pop())
                    print('A --> C')
                else:
                    if stack1[-1] > stack3[-1]:
                        stack1.append[stack3.pop()]
                        print('C --> A')
                    else:
                        stack3.append(stack1.pop())
                        print('A --> C')

            else:
                if len(stack2) == 0:
                    stack2.append(stack3.pop())
                    print('C --> B')
                elif len(stack3) == 0:
                    stack3.append(stack2.pop())
                    print('B --> C')
                else:
                    if stack2[-1] > stack3[-1]:
                        stack2.append(stack3.pop())
                        print('C --> B')
                    else:
                        stack3.append(stack2.pop())
                        print('B --> C')

    elif n % 2 == 1:
        for step in range(1, 2**n):

            if step % 3 == 1:
                if len(stack1) == 0:
                    stack1.append(stack3.pop())
                    print('C --> A')
                elif len(stack3) == 0:
                    stack3.append(stack1.pop())
                    print('A --> C')
                else:
                    if stack1[-1] > stack3[-1]:
                        stack1.append(stack3.pop())
                        print('C --> A')
                    else:
                        stack3.append(stack1.pop())
                        print('A --> C')

            elif step % 3 == 2:
                if len(stack1) == 0:
                    stack1.append(stack2.pop())
                    print('B --> A')
                elif len(stack2) == 0:
                    stack2.append(stack1.pop())
                    print('A --> B')
                else:
                    if stack1[-1] > stack2[-1]:
                        stack1.append(stack2.pop())
                        print('B --> A')
                    else:
                        stack2.append(stack1.pop())
                        print('A --> B')

            else:
                if len(stack2) == 0:
                    stack2.append(stack3.pop())
                    print('C --> B')
                elif len(stack3) == 0:
                    stack3.append(stack2.pop())
                    print('B --> C')
                else:
                    if stack2[-1] > stack3[-1]:
                        stack2.append(stack3.pop())
                        print('C --> B')
                    else:
                        stack3.append(stack2.pop())
                        print('B --> C')


HanoiTower()
