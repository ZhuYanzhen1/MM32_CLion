# -*- coding: utf-8 -*-
"""
Created on Fri Sep 17 21:33:53 2021

@author: mpanicker
"""

import numpy as np
import scipy.signal as ss
from matplotlib import pyplot as plt

# boxcar
# triang
# blackman
# hamming
# hann
# bartlett
# flattop
# parzen
# bohman
# blackmanharris
# nuttall
# barthann
# cosine
# exponential
# tukey
# taylor
# kaiser

def dummy_signal(f_sample,num_sample):
	# a mix of 400Hz and 600Hz signals
	t = np.arange(num_sample)/f_sample
	signal = np.sin(2 * 3.1415 * 400 * t) + 0.1 * np.cos(2 * 3.1415 * 600 * t)
	return signal


def design_filter(cutoff_frequency, f_sample, filter_length, window_function):
	f_nyquist = f_sample /2
	if filter_length%2 == 0:
		filter_length += 1

	if window_function == 'rectangular':
		filter_coefficients = ss.firwin(filter_length, cutoff_frequency / f_nyquist)
	elif window_function != 'kaiser':
		filter_coefficients = ss.firwin(filter_length, cutoff_frequency / f_nyquist, window = window_function)
	elif window_function == 'kaiser':
		filter_coefficients = kaiser_filter(cutoff_frequency, 40, f_sample)
	else:
		filter_coefficients = [0]

	return filter_coefficients

def kaiser_filter(cutoff_frequency, stop_band_atten, f_sample):
	f_nyquist = f_sample / 2
	transition_width = 5 /f_nyquist
	N, beta = ss.kaiserord(stop_band_atten, transition_width)
	filter_coefficients = ss.firwin(N, cutoff_frequency/f_nyquist, window=('kaiser', beta))
	return filter_coefficients

def filter_signal(input_signal, filter_coefficients):
	return ss.lfilter(filter_coefficients, 1, input_signal)

def plot_frequency_response(filter_coefficients, f_sample):
	w,h = ss.freqz(taps,worN = 8000)
	h =  20  * np.log10(np.abs(h))
	w = (w/3.1415) * (f_sample / 2)
	plt.plot(w,h)
	plt.grid(True)
	plt.xlabel('Frequency (Hz)')
	plt.ylabel('Gain')
	plt.title('Frequency Response')
	plt.show()
	#plt.ylim(-0.05, 1.05)


fs = 5000#Hz
fc = 500#Hz
sig = dummy_signal(fs,200)
taps = design_filter(fc,fs,20,'boxcar')
filtered = filter_signal(sig, taps)
plot_frequency_response(taps,fs)
