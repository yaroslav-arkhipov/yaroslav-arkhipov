Description

The project was created as a test task. To use the project, you need the rabbitmq-c library (https://github.com/alanxz/rabbitmq-c). The project was created on Visual Studio 2017.

SendMessage - creates messages and sends them to all listeners.
ListenServer - listens to the advertised port and receives messages from the server and writes them to the queue.
Consumer - subscribes to the queue and reads messages from the queue, and also displays them in the console.

Usage

The launch and use of the program is carried out from the console:
1. Launch the console with the combination "Win + R", type "cmd", click "OK"
2. Go to the directory with executable files with the command "cd% Path_to_folders_Executable files%" and press Enter
3. First start ListenServer with the command "ListenServer hostname port exchange bindingkey queuename". Example: "ListenServer localhost 5672 amq.direct test testqueue".
4. Repeat steps 1 to 3 to initialize the required number of clients.
5. Launch another console by repeating steps 1 and 2.
6. Launch Consumer with the command "Consumer hostname port queuename". Example: "Consumer localhost 5672 testqueue" (the queue name must match the previously declared in ListenServer).
7. Launch another console by repeating steps 1 and 2.
8. Run SendMessage using the command "SendMessage hostname port exchange bindingkey". Example: "SendMessage localhost 5672 mq.direct test".
