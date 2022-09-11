# Instructions

First run `make makedir` to create bin, obj, and sorted folders for each task.

All commands shown are relative to the root directory of the project.

## Creating folders

`make makedir`

Creates the necessary bin, obj, and sorted folders for each task, if not already present.

## Building tasks

`make taskn`

Creates the task binary in bin/taskn, where *n* is the task number (1-4).

## Running tasks

### Task 1

`src/task1/Task1.sh INPUT OUTPUT`

Runs the Task 1 shell script with file INPUT and outputs file OUTPUT.

`bin/task1 INPUT OUTPUT`

Runs the Task 1 executable with file INPUT and outpits file OUTPUT.

### Tasks 2 to 4

`bin/taskn INPUT OUTPUT` (*n* in {1, 2, 3, 4})

Runs Task *n* with file INPUT. OUTPUT refers to the output of Task 1, *not* the final output of the program.

All three tasks will output their files in their respective `/sorted/` subdirectories.