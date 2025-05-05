# netzer_jfet_overdrive
A better overdrive than you'd think

## Eh?
This came about from my dissatisfaction with my 'PolyCule' overdrive - [marvel at its inadequacy!](https://github.com/sirKayAndFriends/PolyCule_VST) - I liked the voice controls but the actual distortion sounded bad.  What was to be done?

Basically the waveshaping was ass.  Before I got into this programming nonsense I made a lot of DIY circuits, especially those from [runoffgroove](https://www.runoffgroove.com/).  I liked their JFET overdrives a lot so off I went to LTSpice to see if I could simulate a single [Fetzer Valve](https://www.runoffgroove.com/fetzervalve.html) stage.  I then used [this method](https://www.ampbooks.com/mobile/dsp/preamp/) from Amp Books to make a transfer function.  I threw that together with some existing code for the tone shaping bits and it sounded good!  

<sub> I think the problem was that the original transfer functions I was using were for diodes - they were ok but not really for me.  I got them from [this](https://dafx.de/paper-archive/2012/papers/dafx12_submission_45.pdf) classic DAFX resource. </sub>

## How work?
- Three JFET stages!  The first is always on - add the others for more silliness!

- Active treble and bass controls at 2000hz and 250hz

- 'voice' sets the tone centre frequency, 'shape' sets the boost or cut at that frequency

## Why does it look like that?
Those old DIY sites looked awesome.  I have a real nostalgia for that old, cruddy internet thing.  There's a thriving community of people making mad things in their sheds and I find those spaces very comforting amid our current internet descent into hell (although all those 90s web hippies did also turn into fascists - I'm aware of that).  

I wanted to bring out some of the runoffgroove, tagboardFX, geofex, make:, DNA for this project!  Thats also why it is called 'Netzer_2.0' (like... FET-zer but internet)
