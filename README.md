
RODOS
=====

Rodos (Real time Onboard Dependable Operating System) is a real-time operating system for embedded systems and was designed for application domains demanding high dependability. 

The first directive of RODOS is **"As simple as possible"**.  
We target the **Irreducible Complexity"** which is very difficult to reach.
Our target is: every developer shall be able to understand the whole code.
This is important to achieve a very low error number.

RODOS can run as "bare metal": directly on the hardware (it is self contained)
or on the top of other operating systems, like Linux, Posix derivates, RTEMS,
FreeRTOS, etc. We have many ports to many different processors, but here
only a few are published. In the future more will come.

An important aspect of Rodos is its integrated real-time middleware.
Developing the control and payload software on the top of a middleware provides
maximal modularity today.
Applications/modules can be developed independently and it is very simple
to interchange modules later without worrying about side effects.

Rodos was implemented as a software framework in C++ with an object
oriented application programm interface (API).
It is organized in layers: The lowest layer (1) is responsible for
control of the embedded system hardware (HAL: Hardware abstraction layer).
The next layer (2) kernel: administrates the local resources, threads and time.
On top of the kernel we have the middleware (layer 3) which enables communication
between building blocks (BB) using a publisher subscriber multicast protocol.
And on the top of the middleware the user may implement his applications (layer 4)
as a distributed software network of simple BBs.
The Building Blocks API on the top of the middleware is a service oriented interface.
BBs interact by providing services to other BBs and using services from other BBs.

To get started asap and try out RODOS on your laptop please look at [our fast intro](README-fast-intro.md).

For further tutorials take a look to the short introduction at  

* [https://en.wikipedia.org/wiki/Rodos_(operating_system)](https://en.wikipedia.org/wiki/Rodos_%28operating_system%29) 
* [https://de.wikipedia.org/wiki/Rodos_(Betriebssystem)](https://de.wikipedia.org/wiki/Rodos_%28Betriebssystem%29)


And take a look to all other README file in this directory and
in each sub directory in recommended read order:


| File                        | Purpose                                           |
| ----------------------------|---------------------------------------------------|
| README.md                   | Project Description                               |
| README-fast-intro.md        | Installation and first Tests, points to Tutorials |
| README-set-tool-chains.md   | Installation Instructions of different Toolchains |
| README-how-to-begin.pdf     | Printable Installation and Test Instructions      |
| doc/rodos-dokumentation.pdf | Printable API Documentation                       |
| doc/*                       | Coding Directives, Usage on STM32F4, Raspberry Pi |
| README-cmake.md             | Guide to build RODOS Projects with CMake          |


Any questions, suggestions or complaints?
please contact

Sergio Montenegro
sergio.montenegro@uni-wuerzburg.de

We want to improve it. We hear your suggestions!

# Our Users

<span>
<img align="left" src="https://gitlab.com/rodos/rodos/-/raw/list-users/doc/logos/Kamaro_Logo.png?inline=false" width="128">
<p>&emsp;&emsp;Kamaro Engineering e.V. - Modular Field Robot Beteigeuze NOVA</p>
</span>
