TARGET = S21_3DViewer_v1.0

CC = gcc
FLAGS = -Wall -Werror -Wextra
SRC = ./Backend/s21_*.c
OBJ = *.o
LFLAGS = -lcheck

ifeq ($(shell uname), Linux) 
	LFLAGS += -pthread  -lsubunit -lrt -lm
endif

all: install

install:
	@echo "Installation of S21_3D_Viewer_v1.0:"
	@mkdir -p ../build
	@cd Frontend/3D_Viewer_v1 && qmake && make && make clean && rm -rf Makefile && mv 3D_Viewer_v1 ../../../build/$(TARGET).app
	@echo "Installation completed! You can find the app in the ../build/ folder!"
	
uninstall:
	@rm -rf ../build
	@echo "Uninstall completed!"
	
dvi:
	open our_readme.md
	
dist: 
	@mkdir -p ../dist
	@cp -R ../build/$(TARGET).app ../dist
	@cp our_readme.md ../dist 
	@cd ../dist && tar cvzf $(TARGET).tgz *
	@cd ../dist && rm -rf $(TARGET).app our_readme.md
	@echo "Archive creation completed successfully!"

tests: clean s21_3d_viewer.a
	$(CC) $(FLAGS) -c ./Tests/viewer_tests.c
	$(CC) viewer_tests.o $(FLAGS) $(LFLAGS) -L. s21_3d_viewer.a -o test

s21_3d_viewer.a:
	$(CC) $(FLAGS) -c $(SRC)
	ar rcs s21_3d_viewer.a $(OBJ)
	ranlib s21_3d_viewer.a
	rm -rf *.o

add_coverage:
	$(eval FLAGS += --coverage)

gcov_report: clean add_coverage tests
	-./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html

clean:
	rm -rf *.o *.a *.gcno *.gcda test test.info report
