"""
Common functions & definitions
"""

from dataclasses import dataclass
from enum import Enum
from hashlib import sha1
import json
import os
from shutil import which
from subprocess import PIPE, run
from sys import executable as PYTHON, platform
from typing import List, Tuple, Union

import yaml
try:
    from yaml import CLoader as Loader
except ImportError:
    from yaml import Loader

#############
# Functions #
#############

def get_file_sha1(path: str) -> bytes:
    """Gets the SHA1 hash of a file"""

    with open(path, 'rb') as f:
        return sha1(f.read()).digest()

def get_cmd_stdout(cmd: str, text=True) -> str:
    """Run a command and get the stdout output as a string"""

    ret = run(cmd.split(), stdout=PIPE, text=text)
    assert ret.returncode == 0, f"Command '{cmd}' returned {ret.returncode}"
    return ret.stdout

class Binary(Enum):
    DOL = 1
    REL = 2

# ppcdis source output
SourceDesc = Union[str, Tuple[str, int, int]]

def get_containing_slice(addr: int) -> Tuple[Binary, SourceDesc]:
    """Finds the binary containing an address and its source file
    Source file is empty string if not decompiled"""

    dol_raw = get_cmd_stdout(f"{SLICES} {DOL_YML} {DOL_SLICES} --containing {addr:x}")
    containing = json.loads(dol_raw)
    return (Binary.DOL, containing)


def lookup_sym(sym: str, dol: bool = False, rel: bool = False, source_name: str = None) -> int:
    """Takes a symbol as a name or address and returns the address"""

    # Get binary
    if dol:
        binary_name = DOL_YML
    else:
        binary_name = None

    # Determine type
    try:
        return int(sym, 16)
    except ValueError:
        return get_address(sym, binary_name, source_name)


def lookup_sym_full(sym: str, dol: bool = False, rel: bool = False, source_name: str = None
                    ) -> int:
    """Takes a symbol as a name or address and returns both the name and address"""

    # Get binary
    if dol:
        binary_name = DOL_YML
    else:
        binary_name = None

    # Determine type
    try:
        return int(sym, 16), get_name(sym)
    except ValueError:
        return get_address(sym, binary_name, source_name), sym


def get_address(name: str, binary: bool = None, source_name: bool = None) -> int:
    """Finds the address of a symbol"""

    args = [name]
    if binary is not None:
        args.append(f"-b {binary}")
    if source_name is not None:
        args.append(f"-n {source_name}")

    raw = get_cmd_stdout(
        f"{SYMBOLSCRIPT} {SYMBOLS} --get-addr {' '.join(args)}")
    return json.loads(raw)


def get_name(addr: int, binary: bool = None, source_name: bool = None) -> int:
    """Finds the name of a symbol"""

    args = [addr]
    if binary is not None:
        args.append(f"-b {binary}")
    if source_name is not None:
        args.append(f"-n {source_name}")

    raw = get_cmd_stdout(
        f"{SYMBOLSCRIPT} {SYMBOLS} --get-name {' '.join(args)}")
    return json.loads(raw)

def find_headers(dirname: str, base=None) -> List[str]:
    """Returns a list of all headers in a folder recursively"""

    if base is None:
        base = dirname

    ret = []
    for name in os.listdir(dirname):
        path = dirname + '/' + name
        if os.path.isdir(path):
            ret.extend(find_headers(path, base))
        elif name.endswith('.h'):
            ret.append(path[len(base)+1:])

    return ret


def load_from_yaml(path: str, default=None):
    """Loads an object from a yaml file"""

    if default is None:
        default = {}
    with open(path) as f:
        ret = yaml.load(f.read(), Loader)
        if ret is None:
            ret = default
        return ret


################
# Project dirs #
################

VERSION = "GOStrikers2013"
OUTNAME = "InazumaWii" # fun fact: in all other versions it's InazmaWii, only took them a few years to notice it

# Include directory
INCDIR = "include"

# Build artifacts directory
BUILDDIR = "build"

# Build include directory
BUILD_INCDIR = f"{BUILDDIR}/include"

# Output binaries directory
OUTDIR = f"out/{VERSION}"

# Original binaries directory
ORIG = f"orig/{VERSION}"

# Tools directory
TOOLS = "tools"

# Config directory
CONFIG = f"config/{VERSION}"

#########
# Tools #
#########

# ppcdis
PPCDIS = "tools/ppcdis"
PPCDIS_INCDIR = f"{PPCDIS}/include"
ANALYSER = f"{PYTHON} {PPCDIS}/analyser.py"
DISASSEMBLER = f"{PYTHON} {PPCDIS}/disassembler.py"
ORDERSTRINGS = f"{PYTHON} {PPCDIS}/orderstrings.py"
ORDERFLOATS = f"{PYTHON} {PPCDIS}/orderfloats.py"
ASSETRIP = f"{PYTHON} {PPCDIS}/assetrip.py"
ASSETINC = f"{PYTHON} {PPCDIS}/assetinc.py"
FORCEFILESGEN = f"{PYTHON} {PPCDIS}/forcefilesgen.py"
ELF2DOL = f"{PYTHON} {PPCDIS}/elf2dol.py"
FORCEACTIVEGEN = f"{PYTHON} {PPCDIS}/forceactivegen.py"
SLICES = f"{PYTHON} {PPCDIS}/slices.py"
PROGRESS = f"{PYTHON} {PPCDIS}/progress.py"
SYMBOLSCRIPT = f"{PYTHON} {PPCDIS}/symbols.py"

# Codewarrior
TOOLS = "tools"

COMPILER_DIR = os.path.join(TOOLS, "compilers")
GC_COMPILER_DIR = os.path.join(COMPILER_DIR, "GC")
WII_COMPILER_DIR = os.path.join(COMPILER_DIR, "Wii")

# also used for nw4r/snd/snd_adpcm.cpp
REVO_EX_CW = os.path.join(GC_COMPILER_DIR, "3.0a5.2")
REVO_EX_CC = os.path.join(REVO_EX_CW, "mwcceppc.exe")

SDK_CW = os.path.join(WII_COMPILER_DIR, "1.0")
SDK_CC = os.path.join(SDK_CW, "mwcceppc.exe")

# NOTE: nw4r most likely uses 1.2(build 158), however the installer is corrupted, this is the closest version available
NW4R_CW = os.path.join(WII_COMPILER_DIR, "1.1")
NW4R_CC = os.path.join(NW4R_CW, "mwcceppc.exe")

MOBICLIP_CW = os.path.join(WII_COMPILER_DIR, "1.5")
MOBICLIP_CC = os.path.join(MOBICLIP_CW, "mwcceppc.exe")

DWC_CW = os.path.join(WII_COMPILER_DIR, "1.6")
DWC_CC = os.path.join(DWC_CW, "mwcceppc.exe")

# TODO: Unknown what compiler is used by the game, haven't tested 1.5 or 1.6 but it's newer than 1.3
CODEWARRIOR = os.path.join(WII_COMPILER_DIR, "1.7")

CC = os.path.join(CODEWARRIOR, "mwcceppc.exe")
LD = os.path.join(CODEWARRIOR, "mwldeppc.exe")
if platform != "win32":
    if(which("wibo") is not None):
        WIN32_WRAPPER = "wibo"
    elif(which("wine") is not None):
        WIN32_WRAPPER = "wine"
    assert WIN32_WRAPPER != "" "Wine or Wibo not found!"
    
    REVO_EX_CC = f"{WIN32_WRAPPER} {REVO_EX_CC}"
    SDK_CC = f"{WIN32_WRAPPER} {SDK_CC}"
    NW4R_CC = f"{WIN32_WRAPPER} {NW4R_CC}"
    MOBICLIP_CC = f"{WIN32_WRAPPER} {MOBICLIP_CC}"
    DWC_CC = f"{WIN32_WRAPPER} {DWC_CC}"
    CC = f"{WIN32_WRAPPER} {CC}"
    LD = f"{WIN32_WRAPPER} {LD}"

# DevkitPPC
DEVKITPPC = os.environ.get("DEVKITPPC")
if DEVKITPPC is None:
    DEVKITPPC = os.path.join(TOOLS, "devkitppc")
    assert(os.path.isdir(DEVKITPPC))

AS = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-as")
OBJDUMP = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-objdump")
CPP = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-cpp")

ICONV = f"{PYTHON} tools/sjis.py" # TODO: get actual iconv working(?)

#########
# Files #
#########

# Slices
DOL_SLICES = f"{CONFIG}/dol_slices.yml"

# Overrides (TODO: do these need to be separate for rel?)
ANALYSIS_OVERRIDES = f"{CONFIG}/analysis_overrides.yml"
DISASM_OVERRIDES = f"{CONFIG}/disasm_overrides.yml"

# Binaries
DOL = f"{ORIG}/{OUTNAME}.dol"  # read in python code
DOL_YML = f"{CONFIG}/dol.yml"
DOL_SHA = f"{ORIG}/{OUTNAME}.dol.sha1"
DOL_OK = f"{BUILDDIR}/{OUTNAME}.dol.ok"
DOL_ASM_LIST = f"{BUILDDIR}/{OUTNAME}.dol.asml"

# Symbols
SYMBOLS = f"{CONFIG}/symbols.yml"

# Assets
ASSETS_YML = f"{CONFIG}/assets.yml"

# Analysis outputs
EXTERNS = f"{BUILDDIR}/externs.pickle"
DOL_LABELS = f"{BUILDDIR}/labels.pickle"
DOL_RELOCS = f"{BUILDDIR}/relocs.pickle"

# Linker
DOL_LCF_TEMPLATE = f"{CONFIG}/dol.lcf"
DOL_LCF = f"{BUILDDIR}/dol.lcf"

# Outputs
DOL_ELF = f"{BUILDDIR}/{OUTNAME}.elf"
DOL_OUT = f"{OUTDIR}/{OUTNAME}.dol"
DOL_MAP = f"{OUTDIR}/{OUTNAME}.map"

# Optional full disassembly
DOL_FULL = f"{OUTDIR}/dol.s"

DOL_SDATA2_SIZE = 4

##############
# Tool Flags #
##############

ASFLAGS = ' '.join([
    "-m gekko",
    f"-I {INCDIR}",
    f"-I {PPCDIS_INCDIR}",
    f"-I orig"
])

INCDIRS = [
    PPCDIS_INCDIR,
    BUILD_INCDIR,
    "include",
    "libs/RVL_SDK/include",
    "libs/NdevExi2A/include",
    "libs/PowerPC_EABI_Support/include",
    "libs/PowerPC_EABI_Support/include/stl",
    "libs/PowerPC_EABI_Support/include/stl/internal",
    # TODO: improve build system to seperate these includes
    "libs/Shade/include", 
    "libs/libMobiclip/include",
    "libs/MoEngineLib",
    "libs/MoEngineLib/Engine",
    "libs/MoEngineLib/Codecs",
    "libs/MoEngineLib/Codecs/Decoder/src",
    "libs/MoEngineLib/Codecs/MoFastaudio/include",
    "libs/MoEngineLib/Codecs/ImaAdpcm/include"
]

MWCC_INCLUDES = ' '.join(f"-i {d}" for d in INCDIRS)
GCC_INCLUDES = ' '.join(f"-I {d}" for d in INCDIRS)

DEFINES = [ 
    "MATCHING"
]

MWCC_DEFINES = ' '.join(f"-d {d}" for d in DEFINES)
GCC_DEFINES = ' '.join(f"-D {d}" for d in DEFINES)

CPPFLAGS = ' '.join([
    "-nostdinc",
    #GCC_DEFINES,
    GCC_INCLUDES
])

# there are a lot of diffrenet libraries that probably need their own libraries, this is just a base
CFLAGS = [
    "-enum int",
    "-enc SJIS",
    "-nodefaults",
    "-fp hard",
    "-proc gekko",
    #"-char signed",
    #MWCC_DEFINES
]

BASE_DOL_CFLAGS = CFLAGS + [
    "-O4,s",
    "-use_lmw_stmw on",
    "-Cpp_exceptions off", # Should be on for shade and game code blegh
    "-ipa file"  # also stupid
]

BASE_RVL_SDK_CFLAGS = CFLAGS + [
    "-O4,p",
    "-Cpp_exceptions off",
    "-inline auto",
    "-ipa file",
    "-func_align 16"
]

BASE_RUNTIME_CLFAGS = CFLAGS + [
    "-O4,p",
    "-use_lmw_stmw on",
    "-str pool,readonly,reuse",
    "-inline on",
    "-func_align 4",  # get rid of 0s
    "-inline off"
]

BASE_METRO_CFLAGS = CFLAGS + [
    "-O4,p",
    "-use_lmw_stmw on",
    "-func_align 4",
    "-Cpp_exceptions off",
    "-inline on"
]

BASE_MSL_C_CFLAGS = BASE_METRO_CFLAGS + [
     "-str pool, readonly, reuse",
     "-ipa file"
]

BASE_NDEVEXI2A_CLFAGS = [
    "-Cpp_exceptions off",
    "-enum int",
    "-inline auto", 
    "-ipa file", 
    "-fp hard", 
    "-O4,p", 
    "-nodefaults", 
    "-func_align 4"
]

BASE_SHD_CLFAGS = BASE_DOL_CFLAGS + [
    "-fp fmadd",
    "-rostr",
    "-lang=c++",
    "-O4,p",
    "-use_lmw_stmw on",
    "-inline on",
    "-func_align 4",  # get rid of 0s
]

BASE_MOBICLIP_CFLAGS = BASE_DOL_CFLAGS + [
    "-RTTI on",
    "-lang=c++",
    "-O4,p",
    "-use_lmw_stmw off",
    "-Cpp_exceptions off"
]

BASE_NW4R_CFLAGS = CFLAGS + [
    "-O4,p",
    "-use_lmw_stmw on",
    "-lang=c++",
    "-inline auto",
    "-Cpp_exceptions off",
    "-RTTI off"
]

LOCAL_CFLAGS = [
    "-proc gekko",
    "-maxerrors 1",
    "-I-",
    MWCC_INCLUDES
]

DOL_CFLAGS = ' '.join(BASE_DOL_CFLAGS + LOCAL_CFLAGS)
RVL_SDK_CFLAGS = ' '.join(BASE_RVL_SDK_CFLAGS + LOCAL_CFLAGS)
RUNTIME_CFLAGS = ' '.join(BASE_RUNTIME_CLFAGS + LOCAL_CFLAGS)
METRO_CFLAGS = ' '.join(BASE_METRO_CFLAGS + LOCAL_CFLAGS)
MSL_C_CFLAGS = ' '.join(BASE_MSL_C_CFLAGS + LOCAL_CFLAGS)
NDEVEXI2A_CLFAGS = ' '.join(BASE_NDEVEXI2A_CLFAGS + LOCAL_CFLAGS)
SHD_CLFAGS = ' '.join(BASE_SHD_CLFAGS + LOCAL_CFLAGS)
MOBICLIP_CFLAGS = ' '.join(BASE_MOBICLIP_CFLAGS + LOCAL_CFLAGS)
NW4R_CFLAGS = ' '.join(BASE_NW4R_CFLAGS + LOCAL_CFLAGS)

LDFLAGS = ' '.join([
    "-fp hard",
    "-w off",
    "-maxerrors 1",
    "-mapunused"
])

PPCDIS_ANALYSIS_FLAGS = ' '.join([
    f"-o {ANALYSIS_OVERRIDES}"
])

PPCDIS_DISASM_FLAGS = ' '.join([
    f"-m {SYMBOLS}",
    f"-o {DISASM_OVERRIDES}"
])

@dataclass
class SourceContext:
    cflags: str
    binary: str
    labels: str
    relocs: str
    slices: str
    sdata2_threshold: int

DOL_CTX = SourceContext(DOL_CFLAGS, DOL_YML, DOL_LABELS, DOL_RELOCS, DOL_SLICES, DOL_SDATA2_SIZE)

####################
# diff.py Expected #
####################

EXPECTED = "expected"
DOL_EXPECTED = f"{EXPECTED}/build/{OUTNAME}.elf"
