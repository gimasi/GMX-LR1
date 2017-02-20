# GMX-LR1
This is LoRaWAN stack for the GMX-LR1 node. It is based on [ST I-CUBE LRWAN ](http://www.st.com/en/embedded-software/i-cube-lrwan.html) , which in turn is based on [Semtech](https://github.com/Lora-net/LoRaMac-node) code.<br>
We find that ST implementation easier to use to add your own application on top of it, which could be not necessarly a simple sensor that transmits data every n-seconds, like the basic example in Semtech's code.<br>
We have further extended the code to make it even easier to add you application.<br>

We are publishing the stack as an Atollic Studio project, you can download the free-version here and start going immediately ( we find Atollic Studio the best compromise between free and 'commercial' ARM IDE ) <br>

The GMX-LR1 is the node we are using in Gimasi for all our LoRa IoT applications, its one of the possible RF-addons for out multiprotocol TUINO maker's IoT platform.

Here is a picture of the node, in this picture there is also it's companion board that make it's very easy to fast prototype your sensor.
![GMX-LR1](/docsgmx-lr1.jpg?raw=true)
