open Settings;

type tone = {
  wave: Osc.wave,
  offset: float,
  gain: float,
};

type track = {
  steps: array(int),
  osc: array(tone),
  env: Envelope.envelope,
  mutable freq: float,
  mutable gain: float,
  mutable attack: float,
  mutable decay: float,
  mutable sustain: float,
  mutable release: float,
  filter: option(Filter.filter),
};

type appState = {
  playing: bool,
  tempo: float,
  activeStep: int,
  tracks: array(track),
  activeTrack: int,
  bitcrusherEnabled: bool,
  delayEnabled: bool,
  filterEnabled: bool,
};

type action =
  | SetPlaying(bool)
  | SetActiveStep(int)
  | UpdateStep(int)
  | UpdateTempo(float)
  | SetActiveTrack(int)
  | SetGain(float)
  | SetFreq(float)
  | SetAttack(float)
  | SetDecay(float)
  | SetSustain(float)
  | SetRelease(float)
  | ToggleBitcrusherEnabled(bool)
  | ToggleDelayEnabled(bool)
  | ToggleFilterEnabled(bool);

let reducer = (action, state) =>
  switch (action) {
  | SetPlaying(p) => {...state, playing: p}
  | SetActiveStep(n) => {...state, activeStep: n}
  | SetActiveTrack(n) => {...state, activeTrack: n}
  | UpdateTempo(n) => {...state, tempo: n}
  | ToggleBitcrusherEnabled(e) => {...state, bitcrusherEnabled: e}
  | ToggleDelayEnabled(e) => {...state, delayEnabled: e}
  | ToggleFilterEnabled(e) => {...state, filterEnabled: e}
  | SetGain(n) =>
    let t = Array.copy(state.tracks);
    let s = t[state.activeTrack];
    s.gain = n;
    {...state, tracks: t};
  | SetFreq(n) =>
    let t = Array.copy(state.tracks);
    let s = t[state.activeTrack];
    s.freq = n;
    {...state, tracks: t};
  | SetSustain(n) =>
    let t = Array.copy(state.tracks);
    let s = t[state.activeTrack];
    s.sustain = n;
    {...state, tracks: t};
  | SetAttack(n) =>
    let t = Array.copy(state.tracks);
    let s = t[state.activeTrack];
    s.attack = n;
    {...state, tracks: t};
  | SetDecay(n) =>
    let t = Array.copy(state.tracks);
    let s = t[state.activeTrack];
    s.decay = n;
    {...state, tracks: t};
  | SetRelease(n) =>
    let t = Array.copy(state.tracks);
    let s = t[state.activeTrack];
    s.release = n;
    {...state, tracks: t};
  | UpdateStep(u) =>
    let t = Array.copy(state.tracks);
    let s = t[state.activeTrack].steps;
    s[u] = s[u] === 1 ? 0 : 1;
    {...state, tracks: t};
  };

let initialAppState = {
  playing: false,
  tempo: 90.,
  activeStep: (-1),
  activeTrack: 0,
  bitcrusherEnabled: false,
  delayEnabled: false,
  filterEnabled: false,
  tracks: [|
    {
      osc: [|{wave: Osc.Sine, offset: 1.0, gain: 1.0}|],
      freq: 21.83,
      gain: 1.0,
      attack: 0.01,
      decay: 0.2,
      sustain: 0.1,
      release: 0.25,
      env: Envelope.create(),
      steps: [|1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0|],
      filter:
        Some(Filter.create(Filter.LowPass, 3000. /. sampleRate, 1.0, 0.0)),
    },
    {
      osc: [|
        {wave: Osc.Sine, offset: (-1.0), gain: 1.0},
        {wave: Osc.Noise, offset: 1.0, gain: 0.75},
      |],
      freq: 65.,
      gain: 0.75,
      attack: 0.01,
      decay: 0.1,
      sustain: 0.1,
      release: 0.5,
      env: Envelope.create(),
      steps: [|0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0|],
      filter:
        Some(Filter.create(Filter.LowPass, 9000. /. sampleRate, 1.0, 0.0)),
    },
    {
      osc: [|
        {wave: Osc.Square, offset: 2., gain: 0.5},
        {wave: Osc.Square, offset: 3., gain: 0.5},
        {wave: Osc.Square, offset: 4.16, gain: 0.5},
        {wave: Osc.Square, offset: 5.43, gain: 0.5},
        {wave: Osc.Square, offset: 6.79, gain: 0.5},
        {wave: Osc.Square, offset: 8.21, gain: 0.5},
      |],
      freq: 55.,
      gain: 0.25,
      attack: 0.01,
      decay: 0.2,
      sustain: 0.1,
      release: 0.9,
      env: Envelope.create(),
      steps: [|0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0|],
      filter:
        Some(Filter.create(Filter.HighPass, 8000. /. sampleRate, 1.0, 0.0)),
    },
    {
      osc: [|
        {wave: Osc.Square, offset: 6.0, gain: 0.25},
        {wave: Osc.Saw, offset: 4.0, gain: 0.25},
      |],
      freq: 130.81,
      gain: 0.25,
      attack: 0.01,
      decay: 0.05,
      sustain: 0.1,
      release: 1.0,
      env: Envelope.create(),
      steps: [|0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0|],
      filter:
        Some(Filter.create(Filter.LowPass, 5000. /. sampleRate, 1.0, 0.0)),
    },
    {
      osc: [|
        {wave: Osc.Noise, offset: 1.0, gain: 0.05},
        {wave: Osc.Sine, offset: 3.0, gain: 0.75},
      |],
      freq: 130.81,
      gain: 0.5,
      attack: 0.01,
      decay: 0.05,
      sustain: 0.1,
      release: 0.5,
      env: Envelope.create(),
      steps: [|0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0|],
      filter:
        Some(Filter.create(Filter.HighPass, 1200. /. sampleRate, 1.0, 0.0)),
    },
  |],
};