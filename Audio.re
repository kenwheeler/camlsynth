open Portaudio;
open Bigarray;
open Settings;
open AppState;

/* Time reference */
let mtime = ref(0.0);
/* Time increment by sample rate */
let mdelta = 1. /. sampleRate;
let currentSample = ref(0.0);

let playing = ref(false);

Random.self_init();
Portaudio.init();

let deviceId = Portaudio.get_default_output_device();
let device = Portaudio.get_device_info(deviceId);

let getStream = () => {
  let outparam =
    Some({
      channels: 2,
      device: deviceId,
      sample_format: format_float32,
      latency: device.d_default_low_output_latency,
    });

  let stream = open_stream(None, outparam, sampleRate, bufferSize, []);

  stream;
};

let fill_ba = (ba, dispatch, appState) => {
  /* time */
  let beatMs = 60000. /. appState.tempo;
  let beatS = beatMs /. 1000.;
  let stepSec = beatS /. 4.;
  let step = floor(stepSec *. sampleRate);
  /* Fill the buffer */
  for (i in 0 to bufferSize - 1) {
    /* if the step matches */
    if (mod_float(currentSample^, step) == 0.0) {
      let stepIndex = int_of_float(currentSample^ /. step);
      /* if the matched step is active */
      Array.iter(
        t => {
          let params = [|0., t.attack, t.decay, t.sustain, t.release|];
          if (stepIndex !== 0 && t.steps[stepIndex - 1] === 1) {
            (t.env)#enterStage(Attack, params);
          };
        },
        appState.tracks,
      );
      dispatch(SetActiveStep(stepIndex));
    };
    if (currentSample^ > step *. 16.) {
      currentSample := 0.0;
    };
    /* Reset mtime if it gets too big */
    if (mtime^ > Float.max_float) {
      mtime := 0.;
    };

    Array.iter(
      t => {
        let params = [|0., t.attack, t.decay, t.sustain, t.release|];
        if ((t.env)#getStage() === Sustain) {
          (t.env)#enterStage(Release, params);
        };
      },
      appState.tracks,
    );

    let data =
      Array.fold_left(
        (acc, t) => {
          let d =
            Array.fold_left(
              (ac2, o) => {
                let params = [|0., t.attack, t.decay, t.sustain, t.release|];
                ac2
                +. Osc.getData(
                     o.wave,
                     t.freq *. o.offset,
                     t.gain *. o.gain,
                     mtime^,
                   )
                *. (t.env)#nextSample(params);
              },
              0.,
              t.osc,
            );
          acc
          +. (
            switch (t.filter) {
            | None => d
            | Some(f) => Filter.process(f, d)
            }
          );
        },
        0.,
        appState.tracks,
      );

    /* Increment time by sample */
    mtime := mtime^ +. mdelta;

    currentSample := currentSample^ +. 1.0;

    let left = [|2 * i|];
    let right = [|2 * i + 1|];
    /* Set the data at the index */
    Genarray.set(ba, left, data);
    Genarray.set(ba, right, data);
  };
};

let stop = stream => {
  playing := false;
  stop_stream(stream);
};
let play = (stream, dispatch, appStateRef) => {
  mtime := 0.;
  playing := true;
  currentSample := 0.;
  /* Create array relative to buffer size */
  let dims = [|2 * bufferSize|];
  let ba = Genarray.create(float32, c_layout, dims);
  /* Fill the array on loop and write */
  while (playing^ === true) {
    fill_ba(ba, dispatch, appStateRef^);
    Portaudio.write_stream_ba(stream, ba, 0, bufferSize);
  };
  ();
};