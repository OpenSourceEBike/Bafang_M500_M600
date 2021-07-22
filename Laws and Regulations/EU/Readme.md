# EU Legal framework for Ebikes using m500 and m600 motors

## DIY Ebike legal requirements

To be in full compliance with the law in the EU, self-build ebikes (including converted bikes) actually do actually need some documentation to be made before using them in the EU.

##### How to build a legally compliant ebike in the EU

In stark contrast to what a lot of people think, bringing a machine (bike in this case) onto the market (selling or giving it away) is not required for the EU to require certification. It's enough to for a machine to be put into service inside the EU. Which means any DIY ebikes does need to comply to the EU "Machine Directive".
Luckily, we can create all legal paperwork ourselves for free.

##### The declaration of conformity (DoC)

The most basic document we need is a "declaration of conformity (DoC)", in which we declare which EU regulations we followed/tested when building the bike. Be carefull here: You also declare yourself liable for any mistakes here, which in practice you already where if you got into an accident anyway.

The most important directives you need to add to the DoC for an ebike would be:
- Directive 2006/42/EC relating to machinery
- Directive 2014/30/EU relating to electromacgnetic compatibility
- Directive 2014/35/EU relating to electrical equipment designed for use within certain voltage limits

When it comes to adding "standards and/or other normative documents and/or technical specifications", it gets a bit more complicated:
If all your electrical components (primarily the motor) are unaltered(!) and not DIY, you can safely add:
- EN-15194:2017 Cycles - Electrically power assisted cycles - EPAC Bicycles

However: 
if you customised the firmware, things get more complicated.
Normally with most standards you could just "self certify" that your modifications comply with the standard. While that is also possible for 15194:2017, this (in practice) isn't because of `4.2.17 Anti-tampering measure`.
Why this affect anyone altering the firmware, is that these anti-tampering measures require that no modification tools for the firmware are available for the end user. Unless you made the tools yourself (and never even shared them), it's very likely that you used tools that are publically available. and hence you can never comply with `4.2.17 Anti-tampering measure`.
It's very likely that this anti-right-to-repair clause is added to the standard under pressure from companies like Bosch. Because legally it isn't required to be included in the standard for compliance with EU law.

So how do we comply to the law?
Well, a "harmonised standard" is just a checklist-tool that guarantees that you comply to certain EU directives. Notice the directives above? Right, by stating compliance with 15194:2017, one ACTUALLY states he declares himself compliant with `Directive 2006/42/EC relating to machinery`. Luckily we already added that one directly, hence it isn't technically required to also list 15194:2017.

`TODO: Add a pre-formatted DoC`

##### Documenting compliance with the directives

We cannot just say we complied with the directives however. We need to prove we do.
How does one do that? 

We are going to make one big file, in which we combine all technical documentation we can find!

- A list of all the parts used in your bike
- Instructions for disposal, most importantly those related to the battery.
- Get the declaration of conformity of ANY part you get get it form. 
- Get wiring diagrams for the electronics (Bafang diagram is included with this repository)
- Get technical drawings (sizes, angles etc) for as many components as you can get (bafang m500 and m600 included in the repo)
- A Risk analysis what things could go wrong with your bike and why the risk is low. For example: The battery could overheat, but the BMS would shut it down

Remember: This document is just to document that you, in good faith, expect you are compliant with above directives.
In case you cannot get some documentation, but can get a declaration of conformity (which factories and sellers are required to submit on request), it is fine to use that do document compliance for that part. As long as it isn't altered significantly.

What is a significant alteration? A change that increases the risks associated with the part and/or changes the character of the risk (for example: Turning a burning hazard into a shock hazard).
This should also show why the risk analysis is relevant!

##### Adding a CE plate

While A CE nameplate isn't required normally, it is when dealing with machinery such as an ebike.
There are many sources to create a small basic CE plate, which should list the following:

- The  name and full address of the manufacturer
- Designation of the machinery (ie the machine category).
- The manufacturer's designation of series or type - which may include a trademark.
- Machine serial number (if any).
- Year of construction (NB it is prohibited to pre-date or post-date the machinery when affixing the CE marking).


##### An end-user manual

Yes you ready it correctly:
A manual is required. It doesn't have to be extremely complicated, but it does need to list CE compliance, list what things the user should do for normal operation and warn the user of any potential hazards

## Disclaimer

Documents in this folder are supplied for research and educational purposes, to investigate the legal complications of opensource ebike-motor firmwares.

In accordance with EU jurisprudence declearing any normative documents "part of the legal framework", they are not subject to copyright when distributed for fair and reasonable educational purposes.
