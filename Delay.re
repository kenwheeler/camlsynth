open Settings;

type delay = {
  mutable duration: float,
  mutable gain: float,
  delayBuffer: array(float),
  mutable currentSample: int,
};

let create = (duration, gain) => {
  duration,
  gain,
  delayBuffer: Array.make(int_of_float(sampleRate *. 2.), 0.),
  currentSample: 0,
};

let process = (d: delay, input: float) => {
  /* Max buffer size */
  let max = int_of_float(sampleRate *. 2.) - 1;
  /* If we're at the end of the sample count, reset */
  if (d.currentSample > max) {
    d.currentSample = 0;
  };

  /* How many samples are we delaying */
  let delayIndex = int_of_float(sampleRate *. 2. *. d.duration);
  /* Get our insert index */
  let dt = d.currentSample + delayIndex;
  /* If the index exceeds the buffer length, insert at beginning with the difference */
  let insertIndex = dt < max + 1 ? dt : dt mod max;

  /* Fill our delay buffer */
  d.delayBuffer[insertIndex] = input;

  /* Mix delay buffer into input signal */
  let output = input +. d.delayBuffer[d.currentSample] *. d.gain;

  /* Increment sample counter */
  d.currentSample = d.currentSample + 1;

  output;
};