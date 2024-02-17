# Linux-Kernel-Process-Management-And-Monitoring-Tool


# 1. Task Monitor 

# Introduction: 
The Task Monitor Kernel Module is developed to enable monitoring of process state information in a Linux environment. It allows users to write process identifiers to a ‘/proc/pid’ file and retrieve information about the associated tasks, such as the command being executed, the process identifier, and the current state. 

# Design Choices: 
  - File System Integration: Utilize the ‘/proc’ file system to create a virtual file (‘/proc/pid’) for interacting with the kernel module.
  - Memory Management: Implement proper memory allocation and deallocation using kmalloc() and kfree() to prevent memory leaks.
  - Kernel Functions: Utilize kernel functions such as pid_task() and find_vpid() to retrieve task information from process identifiers.
    
# Functionality: 
  - Writing to ‘/proc/pid’: Users can write process identifiers to the ‘/proc/pid’ file to initiate monitoring of specific tasks.
  - Reading from ‘/proc/pid’: Reading from the ‘/proc/pid’ file returns information about the task associated with the provided process identifier, including the       command being executed, the process identifier, and the current state.

# Usage Instructions: 
  - Compilation: Compile the kernel module source code using a Makefile to generate the kernel module binary.
  - Loading the Model: Load the kernel module into the kernel using the ‘insmod’ command. 
  - Writing Process Identifiers: Write process identifiers to the ‘/proc/pid’ file using the command ‘echo’ to initiate monitoring of specific tasks.
  - Reading Task Information: Read from the ‘/proc/pid’ file to retrieve information about the monitored tasks, such as the command being executed, the process         identifier, and the current state.
    
# Conclusion: 
The Task Monitor Kernel Module provides a robust mechanism for monitoring process state information in the Linux kernel. By leveraging the ‘/proc’ file system and kernel functions, it offers users the ability to retrieve valuable insights into the execution status of specific tasks, enhancing system monitoring and management capabilities. 


# 2. Process Launcher 

# Introduction: 
The Process Launcher Tool is designed to facilitate the launching of multiple processes in a Linux environment. It allows users to specify executable paths and command-line arguments for each process and monitors the state of the launched processes using the Task Monitor kernel module. 

# Design Choices: 
  - Memory Allocation: Dynamically allocate memory for storing executable paths and command-line arguments using strdup() to prevent memory leaks.
  - Process Launching Mechanism: Use the fork() and execv() system calls to create child processes and execute specified programs, respectively.
  - Integration with Task Monitor Kernel Module: Write the PID of each forked process to the ‘/proc/pid’ file created by the Task Monitor kernel module for             monitoring process state.
    
# Functionality: 
  - Launching Multiple Processes: Users can specify the number of processes to launch, with a maximum limit of 100.
  - Customizable Executable Paths and Arguments: Users can input executable paths and command-line arguments for each process.
  - Monitoring Process State: The tool integrates with the Task Monitor kernel module to monitor the state of each forked process, providing insights into their         execution status.
    
# Usage Instructions: 
  - Compilation: Compile the program using the C compiler GCC, by running the command ‘gcc launcher.c -o launcher’. 
  - Execution: Execute the compiled program by running ‘. /launcher’ in the terminal.
  - Input: Follow the prompts to enter the number of processes to launch, executable paths, and command-line arguments for each process.
  - Monitoring: After launching the processes, observe the process state information displayed by the Task Monitor kernel module in the ‘/proc/pid’.

# Conclusion: 
The Process Launcher Tool provides a user-friendly interface for launching and monitoring multiple processes in a Linux environment. By leveraging system calls and kernel modules, it offers flexibility and efficiency in managing process execution.
