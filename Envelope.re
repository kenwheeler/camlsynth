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
  .
  enterStage: (stage, params) => unit,
  nextSample: params => float,
  getStage: unit => stage,
  resetLevel: unit => unit,
};

let rec find = (~i=0, a, x) =>
  if (a[i] === x) {
    i;
  } else {
    find(~i=i + 1, a, x);
  };

let create = () => {
  let ret: envelope = {
    val minimumLevel = 0.0001;
    val currentStage = ref(Off);
    val currentLevel = ref(0.0001);
    val multiplier = ref(1.0);
    val currentSampleIndex = ref(0);
    val nextStageSampleIndex = ref(0);
    pri calculateMultiplier = (startLevel, endLevel, lengthInSamples) => {
      multiplier :=
        1.0
        +. (log(endLevel) -. log(startLevel))
        /. float_of_int(lengthInSamples);
      ();
    };
    pub resetLevel = () => {
      currentLevel := minimumLevel;
    };
    pub enterStage = (nextStage, params) => {
      currentStage := nextStage;
      currentSampleIndex := 0;
      nextStageSampleIndex :=
        (
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
        currentLevel := 0.;
        multiplier := 1.0;
      | Attack =>
        currentLevel := minimumLevel;
        this#calculateMultiplier(currentLevel^, 1.0, nextStageSampleIndex^);
      | Decay =>
        currentLevel := 1.0;
        this#calculateMultiplier(
          currentLevel^,
          max(params[3], minimumLevel),
          nextStageSampleIndex^,
        );
      | Sustain =>
        currentLevel := params[3];
        multiplier := 1.0;
      | Release =>
        this#calculateMultiplier(
          currentLevel^,
          minimumLevel,
          nextStageSampleIndex^,
        )
      };
      ();
    };
    pub nextSample = params => {
      switch (currentStage^) {
      | Off => ()
      | Sustain => ()
      | _ =>
        if (currentSampleIndex^ == nextStageSampleIndex^) {
          let currentStageIndex = find(stageOrder, currentStage^, ~i=0);
          let newStage = (currentStageIndex + 1) mod 5;
          this#enterStage(stageOrder[newStage], params);
        };
        currentLevel := currentLevel^ *. multiplier^;
        currentSampleIndex := currentSampleIndex^ + 1;
      };
      currentLevel^;
    };
    pub getStage = () => currentStage^
  };
  ret;
};