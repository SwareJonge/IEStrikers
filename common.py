"""
Common functions & definitions
"""

from dataclasses import dataclass
from enum import Enum
from hashlib import sha1
import json
import os
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

    dol_raw = get_cmd_stdout(f"{SLICES} {DOL_YML} {DOL_SLICES} -p {DOL_SRCDIR}/ --containing {addr:x}")
    containing = json.loads(dol_raw)
    return (Binary.DOL, containing)

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

# Directory for decompiled dol code
DOL_SRCDIR = "src"

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

# Codewarrior
TOOLS = "tools"

SDK_CW = os.path.join(TOOLS, "0x4302_145")
SDK_CC = os.path.join(SDK_CW, "mwcceppc")
CODEWARRIOR = os.path.join(TOOLS, "0x4302_213")
# TODO: figure out what compiler is used by the game, mobiclip uses Wii MW 1.5(0x4302_188) which isn't available(closest build i have is build 0x4302_182(some 1.4 build))
# However i think the main game code(GO at least) is compiled with Wii MW 1.7(0x4302_213), DWC is compiled with Wii MW 1.6(0x4302_202)
CC = os.path.join(CODEWARRIOR, "mwcceppc")
LD = os.path.join(CODEWARRIOR, "mwldeppc")
if platform != "win32":
    SDK_CC = f"wine {SDK_CC}"
    CC = f"wine {CC}"
    LD = f"wine {LD}"

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
]
MWCC_INCLUDES = ' '.join(f"-i {d}" for d in INCDIRS)
GCC_INCLUDES = ' '.join(f"-I {d}" for d in INCDIRS)

DEFINES = [ # probably add a flag for European build
    #"DEBUG"
    #"EU_RELEASE"
]
MWCC_DEFINES = ' '.join(f"-d {d}" for d in DEFINES)
GCC_DEFINES = ' '.join(f"-D {d}" for d in DEFINES)

CPPFLAGS = ' '.join([
    "-nostdinc",
    GCC_DEFINES,
    GCC_INCLUDES
])

# there are a lot of diffrenet libraries that probably need their own libraries, this is just a base
CFLAGS = [
    "-lang=c++",
    "-enum int",
    "-enc SJIS",
    "-nodefaults",
    "-Cpp_exceptions off",   
    "-str pool",
    "-Cpp_exceptions off",
    #"-char signed",
    #"-use_lmw_stmw on",
    MWCC_DEFINES
]


BASE_DOL_CFLAGS = CFLAGS + ["O4, s"]

LOCAL_CFLAGS = [
    "-nostdinc",
    "-proc gekko",
    "-maxerrors 1",
    "-I-",
    MWCC_INCLUDES
]
DOL_CFLAGS = ' '.join(BASE_DOL_CFLAGS + LOCAL_CFLAGS)

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
    srcdir: str
    cflags: str
    binary: str
    labels: str
    relocs: str
    slices: str
    sdata2_threshold: int

DOL_CTX = SourceContext(DOL_SRCDIR, DOL_CFLAGS, DOL_YML, DOL_LABELS, DOL_RELOCS, DOL_SLICES, DOL_SDATA2_SIZE)

####################
# diff.py Expected #
####################

EXPECTED = "expected"
DOL_EXPECTED = f"{EXPECTED}/build/{OUTNAME}.elf"
