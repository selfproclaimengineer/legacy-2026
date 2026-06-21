import pygame

def start():
    pygame.init()
    screen = pygame.display.set_mode((100, 100))

def getKey(keyName):
    run = False
    for event in pygame.event.get():
    keyInput = pygame.key.get_pressed()
    myKey = getattr(pygame,'K_{}'.format(keyName))
    if keyInput [myKey]:
        run = True
    pygame.display.update()

    return run

def main():
    if getKey('w'):
        print('Move Forward Succeed')
    if getKey('s'):
        print('Move Backward Succeed')

if __name__ == '__main__':
    start()
    while True:
        main()
