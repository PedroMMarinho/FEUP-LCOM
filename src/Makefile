# name of the program (Minix service)
PROG=proj


.PATH: ${.CURDIR}/labs/
.PATH: ${.CURDIR}/model/ball/
.PATH: ${.CURDIR}/viewer/
.PATH: ${.CURDIR}/States/
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/physics/



# source code files to be compiled
SRCS = main.c graphics.c timer.c utils.c keyboard.c mouse.c kbc.c cueViewer.c lineViewer.c ball.c Game.c resources.c table.c mouseModel.c playingController.c utilities.c cue.c menu.c menuController.c button.c menuViewer.c multiplayerController.c rtc.c serialPort.c queue.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>