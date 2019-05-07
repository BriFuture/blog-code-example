import math
import numpy as np

DEFAULT_FRAMERATE = 44100

import matplotlib.pyplot as pl

DoublePI = math.pi * 2

import wave
import matplotlib.pyplot as plt
import sounddevice as sd
import scipy.stats
import scipy.fftpack

def serial_corr(wave, lag = 1):
    n = len(wave)
    y1 = wave[lag:]
    y2 = wave[:n-lag]
    corr = np.corrcoef(y1, y2, ddof=0)[0, 1]
    return corr

def findIndex(ys, threshold):
    """找到波形 ys 与 y = threshold 的交点，一般来说，从 0 开始 音量会逐渐增大，
    到第一次与 threshold 相交
    """
    l = len(ys)
    index = np.zeros(l, dtype = np.uint16)
    zeroIndex = 0
    for i in range(l - 1):
        # 音量过零
        if (ys[i] - threshold) * (ys[i+1] - threshold) < 0:
            index[zeroIndex] = i
            zeroIndex += 1
    index = index[0:zeroIndex]
    return index


def findIndexWithPreIndex(ys, preIndex, threshold):
    """在前一个阈值索引基础上，向外侧扩展，找到满足 threshold 阈值的新索引
    """
    indexes = np.zeros(len(preIndex), dtype=np.uint16)
    index2 = findIndex(ys, threshold)
    maxIdx = 0
    curIdx = 0
    for i in range(len(preIndex) >> 1 ):
        start = preIndex[i*2]
        end = preIndex[i*2 + 1]
        if start < maxIdx:
            continue

        tmp = index2[np.where(index2 < start)]
        if len(tmp) > 0:
            indexes[curIdx*2] = tmp[-1] 
        else:
            indexes[curIdx*2] = 0

        tmp = index2[np.where(index2 > end)]
        if len(tmp) > 0:
            indexes[curIdx*2+1] = tmp[0]
            maxIdx = tmp[0]
        else:
            indexes[curIdx*2+1] = preIndex[-1]
            maxIdx = preIndex[-1]
        curIdx += 1

    return indexes[0:curIdx * 2]

def zeroCrossRate(wave, framesize, overlap):
    """计算一段波形序列的过零率
    """
    # wlen = len(self.ws)
    # step = self.framesize - self.overlap
    # frameNum = math.ceil(wlen / step)
    # # frameNum = self.framecount
    # zcr = np.zeros((frameNum, 1))
    # for i in range(frameNum):
    #     curFrame = self.ws[i*step: min(i*step+self.framesize, wlen)]
        
    #     # avoid DC bias
    #     curFrame = curFrame - np.mean(curFrame)
    #     zcr[i] = sum(curFrame[:-1]*curFrame[1:] < 0) / (len(curFrame) - 1)

    # return zcr
    wavelen = len(wave)
    # 步长
    step = framesize - overlap
    frameNum = math.ceil(wavelen / step)
    # 保存过零率
    zcr = np.zeros((frameNum, 1))

    for i in range(frameNum-1):
        curFrame = wave[i*step: i*step + framesize]
        # 将均值移到 0 
        curFrame -= np.mean(curFrame)
        zcr[i] = sum(curFrame[:-1]*curFrame[1:] < 0) / framesize

    curFrame = wave[(frameNum - 1) * step: min((frameNum - 1) * step + framesize, wavelen)]
    curFrame -= np.mean(curFrame)
    zcr[frameNum - 1] = sum(curFrame[:-1]*curFrame[1:] < 0) / len(curFrame)
    return zcr

def calcNormalVolume(wave, framesize, overlap, framerate):
    """计算波形的音量，以帧为单位，计算每一帧的音量 v = sum(|w|)
    """
    # 波形长度
    wlen = len(wave)
    step = framesize - overlap
    frameNum = math.ceil(wlen / step)

    # 预分配声音空间
    volume = np.zeros((frameNum, ))

    for i in range(frameNum - 1):
        # 获取第 i 帧数据
        curFrame = wave[i*step: i*step + framesize]
        curFrame = curFrame - np.mean(curFrame)
        # 公式： v = sum(|w|)
        volume[i] = np.sum(np.abs(curFrame))
    
    curFrame = wave[(frameNum - 1)*step: min((frameNum - 1)*step + framesize, wlen)]
    curFrame = curFrame - np.mean(curFrame)
    volume[(frameNum - 1)] = np.sum(np.abs(curFrame))
    
    vts = np.arange(0, len(volume)) * step * (1.0 / framerate)
    return volume, vts

def calcDbVolume(wave, framesize, overlap, framerate):
    wlen = len(wave)
    step = framesize - overlap
    frameNum = math.ceil(wlen / step)

    volume = np.zeros((frameNum, ))
    
    for i in range(frameNum - 1):
        # 
        curFrame = wave[i*step: i*step + framesize]
        curFrame = curFrame - np.mean(curFrame)
        v = np.sum(np.power(curFrame, 2))
        
        volume[i] = 10 * np.log10(v) if v > 0 else 0

    curFrame = wave[(frameNum - 1)*step: min((frameNum - 1)*step + framesize, wlen)]
    curFrame = curFrame - np.mean(curFrame)
    v = np.sum(np.power(curFrame, 2))
    volume[(frameNum - 1)] = 10 * np.log10(v) if v > 0 else 0
    vts = np.arange(0, len(volume)) * step * (1.0 / framerate)
    return volume, vts
    

def dct_iv(ys):
    """analyse
    """
    N = len(ys)
    ts = (0.5 + np.arange(N)) / N
    fs = (0.5 + np.arange(N)) / 2
    args = np.outer(ts, fs)
    M = np.cos(DoublePI * args)
    amps = np.dot(M, ys) / 2
    return amps, fs


def unbias(ys):
    """将 ys 的均值变为 0
    """
    ys -= np.mean(ys)
    return ys

def normalize(ys, amp):
    """将 ys 归一化后 与 amp 相乘
    """
    high, low = abs(max(ys)), abs(min(ys))
    return amp * ys / max(high, low)

################
# Signal
class Signal(object):
    """ Signal can produce Wave
    """
    def __add__(self, other):
        if other == 0:
            return self
        return SumSignal(self, other)
    __radd__ = __add__

    @property
    def period(self):
        return 0.1

    def makeWave(self, duration = 1, start = 0, framerate = 11025):
        n = round(duration * framerate)
        ts = start + np.arange(n) / framerate
        ys = self.evaluate(ts)
        return Wave(ys, ts, framerate = framerate)

    def evaluate(self, ts):
        return []

class SumSignal(Signal):
    def __init__(self, *args):
        self.signals = args

    @property
    def period(self):
        return max(sig.period for sig in self.signals)

    def evaluate(self, ts):
        ts = np.asarray(ts)
        return sum(sig.evaluate(ts) for sig in self.signals)


class _Noise(Signal):

    def __init__(self, amp = 1.0):
        self.amp = amp

    @property
    def period(self):
        return ValueError("Non-periodic signal.")

class UncorrelatedUniformNoise(_Noise):

    def evaluate(self, ts):
        ys = np.random.uniform(-self.amp, self.amp, len(ts))
        return ys


class BrownianNoise(_Noise):

    def evaluate(self, ts):
        dys = np.random.uniform(-1, 1, len(ts))
        ys = np.cumsum(dys)
        ys = normalize(unbias(ys), self.amp)
        return ys

class PinkNoise(_Noise):

    def __init__(self, amp = 1.0, beta = 1.0):
        self.amp = amp
        self.beta = beta

    def makeWave(self, duration = 1, start = 0, framerate = 11025):
        signal = UncorrelatedUniformNoise()
        wave = signal.makeWave(duration, start, framerate)
        spectrum = wave.makeSpectrum()

        # 在频域进行粉红噪声滤波
        spectrum.pinkFilter(self.beta)
        
        wave2 = spectrum.makeWave()
        wave2.unbias()
        wave.normalize(self.amp)
        return wave2

class UncorrelatedGaussianNoise(_Noise):

    def __init__(self, deviation = 1):
        self.deviation = 1

    def evaluate(self, ts):
        ys = np.random.normal(0, self.deviation, len(ts))
        return ys



class SinSignal(Signal):

    def __init__(self, freq=440, amp=1.0, offset=0, func=np.sin):
        self.freq = freq
        self.amp = amp
        self.offset = offset
        self.func = func

    @property
    def period(self):
        return 1.0 / self.freq

    def evaluate(self, ts):
        phases = DoublePI * self.freq * ts + self.offset
        ys = self.amp * self.func(phases)
        return ys

def CosSignal(freq = 440, amp = 1.0, offset = 0):
    return SinSignal(freq, amp, offset, func = np.cos)

def SincSignal(freq = 440, amp = 1.0, offset = 0):
    return SinSignal(freq, amp, offset, func=np.sinc)

class TriangleSignal(Signal):

    def __init__(self, amp = 1.0, freq = 11025, offset = 0):
        self.amp = amp
        self.freq = freq
        self.offset = offset

    def evaluate(self, ts):
        cycles = self.freq * ts + self.offset / DoublePI
        frac, _ = np.modf(cycles)
        ys = np.abs(frac - 0.5)
        ys = normalize(unbias(ys), self.amp)
        return ys
# Signal
#####################

#####################
# Wave
def readFile(filename, dtype=np.float):
    with wave.open(filename, "rb") as f:
        params = f.getparams()
        _, sampwidth, framerate, nframes = params[:4]
        if dtype==np.float:
            if sampwidth == 2:
                dtype = np.float16
            else:
                dtype = np.float32
        elif dtype == np.int:
            if sampwidth == 2:
                dtype = np.int16
            else :
                dtype = np.int32
        str_data = f.readframes(nframes)
        ws = np.frombuffer(str_data, dtype = dtype)
        ws = ws.astype(np.float32)
    ts = np.arange(0, len(ws)) * (1.0 / framerate)
    w = Wave(ws, ts, framerate = framerate)
    w.params = params
    w.nframes = nframes
    w.sampwidth = sampwidth
    w.normalize()
    # w.str_data = str_data
    return w
    
class Wave(object) :
    """表示波形，包含音量（绝对求和/DB）的计算，过零率，每帧的自相关函数计算
    framesize: length of a frame
    volume: 音量或分贝单位的音量

    Wave 包含两个重要的参数：ts 和 ws，分别表示时间序列和波形序列。两个序列中
    是各个确定时刻的时间和对应的波形值。

    Wave 中有帧的概念，一帧包含多个时间点和波形点。相邻两帧之间起始位置相隔的点数为帧的步长。
    相隔的时间为 deltatime = duration / framecount
    """

    def write(self, filename):
        with wave.open(filename, "wb") as f:
            f.setnchannels(self.params[0])
            f.setsampwidth(self.sampwidth)
            f.setframerate(self.params[2])
            f.setnframes(self.params[3])
            f.writeframes(self.ws.tostring())
            f.close()


    def __init__(self, ws, ts = None, framerate = DEFAULT_FRAMERATE):
        """@property: ws, wave values [corresponding to Y axis]
        ts, wave corresponding time sequence
        """
        self.framerate = framerate
        self.ws = ws
        self.color = "blue"
        self.framesize = 256
        self.overlap = 128
        if ts is None:
            self.ts = np.arange(len(ws)) / self.framerate
        else:
            self.ts = ts

    def __len__(self):
        return len(self.ws)

    def __add__(self, other):
        if other == 0:
            return self
        assert self.framerate == other.framerate
        ws = self.ws + other.ws
        return Wave(ws, self.ts, framerate=self.framerate)

    @property
    def start(self):
        return self.ts[0]

    @property
    def end(self):
        return self.ts[-1]

    @property
    def duration(self):
        """波形的持续时间，若为 1s，波形的点数应该和 framerate 相同
        """
        return len(self.ws) / self.framerate

    @property
    def framecount(self):
        """通过 framesize 和 overlap 计算出帧数
        """
        return math.ceil(len(self.ws) / (self.framesize - self.overlap))

    @property
    def framestep(self):
        """每两帧之间的距离
        """
        return self.framesize - self.overlap

    @property
    def deltatime(self):
        """每两帧之间的时间间隔
        """
        return self.duration * (1.0 / self.framecount)


    def hamming(self):
        self.ws *= np.hamming(len(self.ws))

    def segment(self, timestart, timeend):
        """获取分段波形
        @return Wave
        """
        index = np.where(self.ts > timestart)
        start = index[0][0]

        index = np.where(self.ts < timeend)
        end = index[0][-1]
        wav = Wave(self.ws[start:end+1], framerate= self.framerate)
        return wav

    def makeSpectrum(self):
        """生成频谱
        """
        n = len(self.ws)
        d = 1 / self.framerate

        hs = np.fft.rfft(self.ws)
        fs = np.fft.rfftfreq(n, d)
        return Spectrum(hs, fs, self.framerate)

    def normalize(self, amp = 1.0):
        ws = self.ws
        high, low = abs(max(ws)), abs(min(ws))
        self.ws = ws * (amp / max(high, low))

    def unbias(self):
        self.ws -= np.mean(self.ws)

    def plot(self, start = 0.0):
        plt.plot(self.ts, self.ws + start, self.color)

    def play(self):
        # framerate / nchannels
        sd.play(self.ws, self.framerate)


    def zeroCR(self):
        """过零率按照每一帧进行计算
        每一帧中，波形越过零点的个数
        """
        return zeroCrossRate(self.ws, self.framesize, self.overlap)

    def relation(self, i):
        """计算第 i 帧的自相关函数
        """
        # k from 0 to framecount - 1
        num = self.framesize
        step = self.framesize - self.overlap
        relation = np.zeros((num,), dtype = np.float32)
        if (i+1) * num > len(self.ws):
            return
        wlen = max(len(self.ws), i * step + num)
        w = self.ws[i*step:wlen]
        for k in range(num):
            ## method 1
            for m in range(num - k):
                relation[k] += w[m] * w[m + k]
            ## method 2
            # relation[k] = serial_corr(w, k)
        return relation

    def makeVolume(self, framesize = 256, overlap = 128):
        self.framesize = framesize
        self.overlap = overlap

    def normalVolume(self):
        """计算波形的音量，以帧为单位，计算每一帧的音量 v = sum(|w|)
        """
        # 波形长度
        
        volume, vts = calcNormalVolume(self.ws, self.framesize, self.overlap, self.framerate)
        return volume

    def dbVolume(self):
        """以 DB 为单位计算波形的音量：dbVolume = 10 log10(v^2)
        """
        volume, vts = calcDbVolume(self.ws, self.framesize, self.overlap, self.framerate)
        return volume

    def plotVolume(self):
        plt.plot(self.vts, self.volume)

    def makeDct(self):
        amps, fs = dct_iv(self.ws)
        return Dct(amps, fs, self.framerate)


##################
# Spectrum
class Spectrum(object):
    """Spectrum 谱和波形可以互换
    """
    def __init__(self, hs, fs, framerate, full=False):
        self.hs = hs
        self.fs = fs
        self.framerate = framerate
        self.full = full

    @property
    def max_freq(self):
        return self.framerate / 2

    @property
    def amps(self):
        return np.abs(self.hs)

    @property
    def power(self):
        return self.amps ** 2

    @property
    def real(self):
        return np.real(self.hs)

    @property
    def imag(self):
        return np.imag(self.hs)

    @property
    def angles(self):
        return np.angle(self.hs)

    def plot(self):
        plt.plot(self.fs, self.amps)
        plt.show()

    def play(self):
        """No use
        """
        sd.play(abs(self.hs))

    def lowPass(self, cutoff, factor = 0.0):
        self.hs[abs(self.fs) > cutoff] *= factor

    def highPass(self, cutoff, factor = 0.0):
        self.hs[abs(self.fs) < cutoff] *= factor

    def bandStop(self, low_cutoff, high_cutoff, factor = 0):
        fs = abs(self.fs)
        indices = (low_cutoff < fs) & (fs < high_cutoff)
        self.hs[indices] *= factor

    def pinkFilter(self, beta = 1):
        denom = self.fs ** (beta / 2.0)
        denom[0] = 1
        self.hs /= denom
    
    def makeWave(self):
        if self.full:
            ys = np.fft.ifft(self.hs)
        else:
            ys = np.fft.irfft(self.hs)

        return Wave(ys, framerate = self.framerate)

    def makeIntegratedSpectrum(self):
        cs = np.cumsum(self.power)
        cs /= cs[-1]
        return IntegratedSpectrum(cs, self.fs)

    def estimateSlope(self):
        x = np.log(self.fs[1:])
        y = np.log(self.power[1:])
        t = scipy.stats.linregress(x, y)
        return t

class Dct(Spectrum):

    @property
    def amps(self):
        return self.hs

    def __add__(self, other):
        if other == 0:
            return self

        assert self.framerate == other.framerate
        hs = self.hs + other.hs
        return Dct(hs, self.fs, self.framerate)

    def makeWave(self):
        # N = len(self.hs)
        # ys = scipy.fftpack.idct(self.hs, type = 2) / 2 / N

        # return Wave(ys, framerate = self.framerate)
        ys, fs = dct_iv(self.hs)
        ys *= 2
        return Wave(ys, framerate = self.framerate)

class IntegratedSpectrum:

    def __init__(self, cs, fs):
        self.cs = cs
        self.fs = fs

    def plotPower(self, low = 0, high = None, expo = False):
        cs = self.cs[low:high]
        fs = self.fs[low:high]
        
        if expo:
            cs = np.exp(cs)

        plt.plot(fs, cs)

    def estimateSlope(self, low = 1, high = -12000):
        x = np.log(self.fs[low:high])
        y = np.log(self.cs[low:high])
        t = scipy.stats.linregress(x, y)
        return t
# Spectrum
###################1
