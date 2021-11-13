
# Welcome to the YASL project

![YASL](images/YASL1.png)

This is the firmware source code for YASL (**Y**et **A**nother **S**helf **L**amp) as published on [Thingiverse](https://www.thingiverse.com/thing:5135874) and [Thangs](https://thangs.com/technik.gegg).

This project uses an ESP8266 (Wemos D1 mini) as the controller to address and program the WS2812 (NeoPixel) LEDs. The light effects can be controlled either by a IR remote controller (RC5) or via web interface.

To compile this firmware and program it into the WEMOS D1 mini, you have to load it up in PlatformIO and run the Build/Upload commands of the WEMOS_D1 Project Task.
For the very first build you also need to "Upload Filesystem Image", since the file system contains all the files to run the web server.

It's recommended starting the "**Monitor**" right after the first upload of the firmware in order to retrieve the specific device name (YASL-xxxxxx).
Then take your specific IR remote control (any device that produces RC5 codes is suitable), point it to the IR-Receiver and press the buttons. The debug console will spit out the codes received, which you have to adopt in order to make your remote control work with the YASL.
Once you have written down your codes, open **Config.h** and replace the (hex) codes, as shown below.

```C
// RC5 Codes for IR Remote
#define IR_UP                       0x20    // Arrow UP Button
#define IR_DOWN                     0x21    // Arrow DOWN Button
#define IR_LEFT                     0x11    // Arrow LEFT Button
#define IR_RIGHT                    0x10    // Arrow RIGHT Button
#define IR_MENU                     0x2C    // Menu Button
#define IR_AVTV                     0x2B    // AV Button
#define IR_POWER                    0x0C    // Power Button
#define IR_MUTE                     0x0D    // Mute Button
#define IR_PIP                      0x0F    // Picture-In-Picture Button
#define IR_PICTURE                  0x38    // Picture Button
#define IR_CALENDAR                 0x1C    // Calendar Button
#define IR_PSIZE                    0x1E    // Picture Size Button
#define IR_MINUS                    0x0A    // Minus Button
```

Keep in mind that I've implemented this code using a remote for an TFT monitor I had laying around and thus it might not match the one you have. Although, you should be able finding most of the names defined above on your remote as well. If not, remember: *Names are smoke and mirrors!*

After this, re-compile and re-upload the firmware.

In order to connect to the web interface, first connect to the "*YASL-xxxxxx*" AP. Then use the browser and start the web interface of the WiFi-Manager on IP-Address **192.168.4.1**.
This will start the [WiFi-Manager](https://github.com/tzapu/WiFiManager.git) which enables you to pick your local AP and enter your WiFi credentials. Once you've saved these credentials, you'll be able to connect via "http://yasl-xxxxxx.local" from your browser. Whereas the **xxxxxx** stands for the last 3 bytes of the MAC address of the ESP8266. This will show you this web page, which basically lists the effect and the currently selected one.

![YASL](images/WebInterface.jpg)

Please be aware, that if you have more than one device connected to your network, they all will stay in sync with the selected effect if controlled via the web interface or remote control. If you don't want this behavior for some reason, simple don't connect the YASL to your local network.

The effect you can choose from ar very well know in the NeoPixel universe. Most of them were initailly made by [Mark Kriegsman](https://github.com/kriegsman) and [Andrew Tuline](https://tuline.com), which have posted all their knowledge on the Internet.
Both fellas are a great source, if your going to implement your own effects.

For this special case, all effects have been separated into their own source/header files to make it easier finding and modifying them.
If you're about adding your own effects, simply add them in the *allEffects* collection of *effectlist.cpp*, as shown below.

```C
Effect allEffects[]  = {
    { E_FADE,           "Fade In/Out",        _FadeInOut },
    { E_MUSIC,          "Music",              _Music },
    { E_RGBLOOP,        "RGB Loop",           _RgbLoop },
    { E_CYLONBOUNCE,    "Cylon Bounce",       _CylonBounce },
    { E_METEORRAIN,     "Meteor Rain 1",      _MeteorRain1 },
    { E_TWINKLE,        "Twinkle 1",          _TwinkleRandom },
    { E_RGBNOISE,       "RGB Noise",          _FillNoise8 },
    { E_RIPPLE,         "Ripple 1",           _RipplePal },
    { E_BLENDWAVE,      "Blend Wave",         _BlendWave },
    { E_RUNNINGLIGHTS,  "Running Lights",     _RunningLights },
    { E_COLORWIPE,      "Color Wipe",         _ColorWipe },
    { E_RAINBOW,        "Rainbow",            _RainbowCycle },
    { E_THEATERCASE,    "Theater Chase 1",    _TheaterChase },
    { E_THEATERCASE2,   "Theater Chase 2",    _TheaterChaseRainbow },
    { E_FIRE,           "Fire",               _Fire },
    { E_FIRE2,          "Plasma Fire",        _Fire2 },
    { E_BOUNCINGBALLS,  "Bouncing Balls",     _BouncingColoredBalls },
    { E_METEORRAIN2,    "Meteor Rain 2",      _MeteorRain2 },
    { E_CYLONHUE,       "Cylon Hue",          _CylonHue },
    { E_RIPPLE2,        "Ripple 2",           _Ripple },
    { E_TWINKLE2,       "Twinkle 2",          _Twinkle },
    { E_WHITE,          "Solid White",        _SolidWhite  },
    { E_BLUE,           "Solid Blue",         _SolidBlue },
    { E_GREEN,          "Solid Green",        _SolidGreen },
    { E_YELLOW,         "Solid Yellow",       _SolidYellow },
    { E_RED,            "Solid Red",          _SolidRed },
    { E_ORANGE,         "Solid Orange",       _SolidOrange },
    { E_PINK,           "Solid Pink",         _SolidPink },
    { E_AQUA,           "Solid Aqua",         _SolidAqua },
    { E_PURPLE,         "Solid Purple",       _SolidPurple }
};
```

Please notice that the "displayName" defined here is the one shown on the button of the web interface.

Also, don't forget to add your effect to the enumeration in file *effectlist.h* **before** MAX_EFFECTS:

```C
enum _FX {
    E_FADE,
    E_MUSIC,
    ...
    E_PURPLE,
    MAX_EFFECTS
};

```

[![Open in VS-Code](https://open.vscode.dev/badges/open-in-vscode.svg)](https://open.vscode.dev/technik-gegg/YASL/master)

---

## Recent changes

**1.0.0** - Initial commit
