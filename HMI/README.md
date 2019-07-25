# HMI

## emvironment
### Related softwares:
- MMDAgent : [http://www.mmdagent.jp/](http://www.mmdagent.jp/)
- VisualStudio 2015 community (Different versions make it difficult to compile. I suggest to use the same version of VisualStudio.)


## MMDAgent (main program)
### base files
- Version : `MMDAgent version 1.7 (December 25, 2016)`
- Download : [http://sourceforge.net/projects/mmdagent/files/MMDAgent/MMDAgent-1.7/MMDAgent_win32-1.7.msi/download](http://sourceforge.net/projects/mmdagent/files/MMDAgent/MMDAgent-1.7/MMDAgent_win32-1.7.msi/download)

### files we customized (Plugin & Library)
The contents of the following directories in the `forPlugin` directory can be used by putting them in the official `MMDAgent-1.7`.


|  dir in this repository | contents |
| ---- | ---- |
|Plugin_3DmapReceive    | Receive building name from 3D map by OSC communication |
|Plugin_Eyes            | Get gaze information from shared memory |
|Plugin_hikariAgent     | Light up optical devices |
|Plugin_HMI             | Communicate with an autonomous car |
|Plugin_Kinect          | Pointing recognition |
|Plugin_Multimodal      | Integrated understanding of speech, pointing and gaze recognition |
|Plugin_VAD             | VVAD(Visual VAD) Determine if the driver is speaking |
|Plugin_Julius          | Acquisition time of speech recognition · Added a function to set VVAD start / end flag |
|Library_MMDAgent       | Add variables that can be used throughout MMDAgent |


### files we customized (AppData)
The contents of the following directories in the `forAppData` directory can be used by putting them in the official `MMDAgent-1.7/AppData`.


|  dir in this repository | contents |
| ---- | ---- |
| _jconf_backup | .jconf files are backup |
| nakagawa_HMI | Japanese language model for julius |
| nakagawa_HMI_eng | English language model for julius |
| nakagawa_HMI_デモ用 | Japanese language model for julius (for demonstration version) |
| jconf.txt (file) | config file for julius |




## MMDAgent sample script (model and other files)
### base files
- Version : `MMDAgent "Sample Script" version 1.6`
- Download : [https://sourceforge.net/projects/mmdagent/files/MMDAgent_Example/MMDAgent_Example-1.6/MMDAgent_Example-1.6.zip/download](https://sourceforge.net/projects/mmdagent/files/MMDAgent_Example/MMDAgent_Example-1.6/MMDAgent_Example-1.6.zip/download)

### files we customized
The contents of the following directories in the `forSampleScript` directory can be used by putting them in the official `MMDAgent_Example-1.6`.

|  dir in this repository | contents  |
| ---- | ---- |
|  JapaneseScenario  | Dialog scenario with Japanese input and output |
|  EnglishScenario  |  Dialog scenario with English input and output  |
| EnglishScenario_HumanVoice  | Dialog scenario with English input and output. And the voice output is a human voice recording.  |


## for run the MMDAgent
### files we customized
The contents of the following directories in the `batFileExample` directory can be used to run the MMDAgent.

|  dir in this repository | contents  |
| ---- | ---- |
| MMDA_jp.bat  | .bat file for run the Japanese Ver MMDAgent scenario. |
| MMDA_en.bat  | .bat file for run the English Ver MMDAgent scenario. |


### Description
```dosbatch
cd C:\Users\speech\Desktop\MMDAgent-1.7\Release\AppData\Julius
del jconf.txt
copy .\_jconf_backup\jconf_ja_JP.txt .\jconf.txt

C:\Users\speech\Desktop\MMDAgent-1.7\Release\MMDAgent.exe C:\Users\speech\Desktop\MMDAgent\JapaneseScenario\MMDAgent_Example.mdf
```

- line 1-3 : Specify path of AppData for julius in MMDAgent.
- line 5 : The path of MMDAgent exe file and the location of .mdf as argument are specified.







## gaze
by AISIN [https://www.aisin.co.jp/](https://www.aisin.co.jp/)