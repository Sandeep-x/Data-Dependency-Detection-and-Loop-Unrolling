# Data-Dependency-Detection-and-Loop-Unrolling
Data dependency refers to the relationship between instructions in a program, indicating that the execution of one instruction relies on the availability or output of another instruction. There are three main types of data dependencies: Read-after-write (RAW), Write-after-read (WAR), and Write-after-write (WAW). Detecting data dependencies helps ensure correct execution order and maintain data integrity. This project finds the True data depency i.e. RAW present in the code.

Loop unrolling is an optimization technique used to improve the performance of loops in code. It involves replicating loop iterations to reduce the overhead of loop control mechanisms. This project also generates a Optimized loop unrolled code given a normal program which reduce loop overhead and enhance instruction-level parallelism, leading to potentially faster and more efficient execution.
<br>

## Execution Steps
### Input Data
An input txt file containing assembly level instructions of a certain format is given. <b>Input.txt</b> is the file given as input. Specific format of the instructions given in the file <b>Instruction_Format.txt</b>. Make sure your input file have instructions of the given formats.

### Running the code
Keep the code file and the input text file in the same directory. Run the c++ code. It will generate the dependecy information on output console. It also produce optimized loop unrolled code. 
<br>

## Output
True data dependecny (RAW) among the instructions is displayed on the console. Example shown on below figure.<br>

![image](https://github.com/Sandeep-x/Data-Dependency-Detection-and-Loop-Unrolling/assets/18510244/ba4649f1-fd95-4e39-a082-e8935fd136a4)

This program also generates 4 output code files with optimized code using loop unrolling and code schedulling. Code scheduling is an optimization technique used to rearrange the order of instructions in a program to improve performance. The objective of code scheduling is to minimize stalls and maximize the utilization of available resources. Loop unrolling is an optimization technique used to improve the performance of loops in code.

### Generated output files:

<b> Unscheduled File : </b> This file includes the original instructions from the input file, along with stall information indicating the presence of delays caused by data dependencies between instructions.
<br><br>
<b> Scheduled File : </b> This file represents an optimized version of the code achieved through code scheduling strategies. This optimization approach effectively reduces the number of stalls compared to an unscheduled strategy. The scheduling process utilizes dependency information to intelligently order instructions, minimizing delays caused by data dependencies.
<br><br>
<b> Unrolled_Unscheduled File : </b> 
This file contains an optimized version of loop-unrolled instructions that have been enhanced using the loop unrolling technique. The primary objective of this optimization is to improve the performance of the loop by reducing the overhead associated with loop control mechanisms. By unrolling the loop, the code achieves better utilization of hardware resources and potentially enhances the overall efficiency and execution speed of the loop.
<br><br>
<b> Unrolled_Scheduled File : </b>
This file demonstrates optimization through a combination of loop unrolling and code scheduling strategies. This optimization approach significantly reduces the number of clock cycles required to execute the entire program, resulting in improved efficiency. Moreover, the loop execution is specifically enhanced, allowing for better utilization of hardware resources and further performance gains.
