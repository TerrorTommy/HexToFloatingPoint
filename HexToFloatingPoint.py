def MantissaToFloat(strBin):
    toFpart = False
    fPart = []
    iPart = []
    for s in strBin:
        if s == '.':
            toFpart = True
        if toFpart == False:
            iPart += [s]
        if toFpart == True and s != '.':
            fPart += [s]
    iPart = ''.join(iPart)
    fPart = ''.join(fPart)
    sumfloat = 0
    sumInt = 0
    for idx, s in enumerate(iPart):
        sumInt += int(s) * 2**(len(iPart) - (idx + 1))
    for idx, s in enumerate(fPart):
        sumfloat += int(s) * 2**(-(idx + 1))
    return sumfloat + sumInt


def hex2float(hexTxt):
    bin = "{0:08b}".format(int(hexTxt, 16))
    return bin


def Hex2Float(strHex):
    strNew = [s for s in strHex if s != " "]
    str = "".join(strNew)
    strSep = ""
    strlist = []
    count = 0
    for i in str:
        strSep += i
        count += 1
        if count == 2:
            count = 0
            strlist.append(strSep)
            strSep = ""
    binList = list(map(hex2float, strlist))
    binStr = "".join(binList)
    exponent = binStr[1:9]
    mantisa = binStr[9:]
    actualExpo = 2**(int(exponent, 2) - 127)
    mantisa = '1.' + mantisa
    actualMantisa = MantissaToFloat(mantisa)
    signBit = 0
    if binStr[0] == '0':
        signBit = 1
    elif binStr[0] == '1':
        signBit = -1
    return signBit * actualExpo * actualMantisa
print("here it is")
print(Hex2Float('be7ae0f4'))
