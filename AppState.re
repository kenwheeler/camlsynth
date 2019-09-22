type track = {
  steps: array(int),
  osc: array(Osc.osc),
  env: Envelope.envelope,
};

type appState = {
  playing: bool,
  activeStep: int,
  tracks: array(track),
  activeTrack: int,
};

type action =
  | SetPlaying(bool)
  | SetActiveStep(int)
  | UpdateStep(int)
  | SetActiveTrack(int);

let reducer = (action, state) =>
  switch (action) {
  | SetPlaying(p) => {...state, playing: p}
  | SetActiveStep(n) => {...state, activeStep: n}
  | SetActiveTrack(n) => {...state, activeTrack: n}
  | UpdateStep(u) =>
    let t = Array.copy(state.tracks);
    let s = t[state.activeTrack].steps;
    s[u] = s[u] === 1 ? 0 : 1;
    {...state, tracks: t};
  };

let kickOsc = Osc.create(Osc.Sine, 27.5, 0.75);
let kickEnv = Envelope.create(0.01, 0.25, 0.1, 0.25);

let snareOsc = Osc.create(Osc.Noise, 55., 0.25);
let snareEnv = Envelope.create(0.01, 0.1, 0.1, 0.2);
let snareOsc2 = Osc.create(Osc.Sine, 55., 0.75);

let hatOsc = Osc.create(Osc.Noise, 55., 0.2);
let hatEnv = Envelope.create(0.01, 0.01, 0.015, 0.025);

let initialAppState = {
  playing: false,
  activeStep: (-1),
  activeTrack: 0,
  tracks: [|
    {
      osc: [|kickOsc|],
      env: kickEnv,
      steps: [|1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0|],
    },
    {
      osc: [|snareOsc, snareOsc2|],
      env: snareEnv,
      steps: [|0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0|],
    },
    {
      osc: [|hatOsc|],
      env: hatEnv,
      steps: [|0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0|],
    },
  |],
};