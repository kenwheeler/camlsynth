open Settings;

type stage =
  | Off
  | Attack
  | Decay
  | Sustain
  | Release;

type params = array(float);

let stageOrder = [|Off, Attack, Decay, Sustain, Release|];

type envelope = {
  minimumLevel: float,
  mutable currentStage: stage,
  mutable currentLevel: float,
  mutable multiplier: float,
  mutable currentSampleIndex: int,
  mutable nextStageSampleIndex: int,
};

let create = () => {
  minimumLevel: 0.0001,
  currentStage: Off,
  currentLevel: 0.0001,
  multiplier: 0.1,
  currentSampleIndex: 0,
  nextStageSampleIndex: 0,
};

let rec find = (~i=0, a, x) =>
  if (a[i] === x) {
    i;
  } else {
    find(~i=i + 1, a, x);
  };

let calculateMultiplier = (env, startLevel, endLevel, lengthInSamples) => {
  env.multiplier =
    1.0
    +. (log(endLevel) -. log(startLevel))
    /. float_of_int(lengthInSamples);
};

let enterStage = (env, nextStage, params) => {
  env.currentStage = nextStage;
  env.currentSampleIndex = 0;
  env.nextStageSampleIndex = (
    switch (nextStage) {
    | Off => 0
    | Sustain => 0
    | _ =>
      let idx = find(stageOrder, nextStage, ~i=0);
      int_of_float(params[idx] *. sampleRate);
    }
  );
  switch (nextStage) {
  | Off =>
    env.currentLevel = 0.;
    env.multiplier = 1.0;
  | Attack =>
    env.currentLevel = env.minimumLevel;
    calculateMultiplier(env, env.currentLevel, 1.0, env.nextStageSampleIndex);
  | Decay =>
    env.currentLevel = 1.0;
    calculateMultiplier(
      env,
      env.currentLevel,
      max(params[3], env.minimumLevel),
      env.nextStageSampleIndex,
    );
  | Sustain =>
    env.currentLevel = params[3];
    env.multiplier = 1.0;
  | Release =>
    calculateMultiplier(
      env,
      env.currentLevel,
      env.minimumLevel,
      env.nextStageSampleIndex,
    )
  };
  ();
};

let nextSample = (env, params) => {
  switch (env.currentStage) {
  | Off => ()
  | Sustain => ()
  | _ =>
    if (env.currentSampleIndex == env.nextStageSampleIndex) {
      let currentStageIndex = find(stageOrder, env.currentStage, ~i=0);
      let newStage = (currentStageIndex + 1) mod 5;
      enterStage(env, stageOrder[newStage], params);
    };
    env.currentLevel = env.currentLevel *. env.multiplier;
    env.currentSampleIndex = env.currentSampleIndex + 1;
  };
  env.currentLevel;
};