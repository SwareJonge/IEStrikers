# IEStrikers
Decompilation of Inazuma Eleven Strikers
## Building
### Requirements
- DevkitPPC r38 or r41, other versions are not tested
> **Note**
> on windows the environment variables get set incorrectly for devkitpro, 
> change it to the path where you stored devkitPro
- Ninja(Optional: add ninja to your environment variables)
- Python 3.8+

### Instructions
- Dump a copy of the Inazuma Eleven Go Strikers 2013 and extract main.dol
- rename main.dol to InazumaWii.dol
- clone the repo using `git clone https://github.com/SwareJonge/IEStrikers/`
- Place InazumaWii.dol in `orig/GOStrikers2013/`
- Place the required compilers in `tools`
- (first time only) Install the modules from requirements.txt (`pip install -r requirementstxt`)
- Run configure.py
- Run ninja

## Credits
- [Seeky](https://github.com/SeekyCt) for helping me setting it all up

