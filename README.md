# cbsexploder

Implementation of a CBS client for offline servicing. Capable of installing packages to a foundation image built with [sxsfounder](https://github.com/WitherOrNot/sxsfounder).

> DISCLAIMER: Do not attempt to use this codebase for online servicing. This codebase contains minimal error handling and no recovery features. Any and all damages incurred through use of this software are indication of a skill issue, rip bozo.

## Usage

Requires servicing stack DLLs in same directory.

```
Usage: cbsx /<mode> /m:<path> /o:<path> [/log:<path>] [/dbg] [/?] [/b:<path>]

Modes:
/sp     Stage package
/ip     Install package
/up     Uninstall package
/ep     Enumerate installed packages (overrides other arguments)

Session arguments:
/m      Manifest path for package
/o      Offline image Windows directory path

Optional:
/log    Set log path (default: C:\cbsexploder.log)
/dbg    Enable debug mode (more verbose output and logging)
/?      Print this usage guide
/b      Use batch file

Batch file:
/b argument cannot be used with /sp, /ip, /up, /ep, or /m.
Each line in batch file represents one package operation, in the same format as the above arguments.

Example batch file:
/ip /m:C:\pkg1.mum
/sp /m:C:\pkg2.mum
```

## Credits
 - [asdcorp](https://github.com/asdcorp) - Testing, assistance, guidance, and naming inspiration
 - [seven-mile](https://github.com/seven-mile) - For [CallCbsCore](https://github.com/seven-mile/CallCbsCore), which much of this code is based on
