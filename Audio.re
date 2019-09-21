open Portaudio;
open Bigarray;
open Settings;

/* Time reference */
let mtime = ref(0.0);
/* Time increment by sample rate */
let mdelta = 1. /. sampleRate;
let currentSample = ref(0.0);

let playing = ref(false);

Random.self_init();
Portaudio.init();
let osc1 = Osc.create(Osc.Sine, 27.5, 0.75);
let env1 = Envelope.create(0.01, 0.25, 0.1, 0.5);

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

let fill_ba = (ba, setStep) => {
  /* time */
  /* Fill the buffer */
  for (i in 0 to bufferSize - 1) {
    if (mod_float(currentSample^, step) == 0.0) {
      Console.log(int_of_float(currentSample^ /. step));
      setStep(int_of_float(currentSample^ /. step));
    };
    if (currentSample^ > step *. 16.) {
      currentSample := 0.0;
    };
    /* Reset mtime if it gets too big */
    if (mtime^ > Float.max_float) {
      mtime := 0.;
    };

    if (env1#getStage() === Off) {
      env1#enterStage(Attack);
    };

    if (env1#getStage() === Sustain) {
      env1#enterStage(Release);
    };
    let data = osc1#getData(mtime^) *. env1#nextSample();

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
let play = (stream, steps, setStep) => {
  mtime := 0.;
  playing := true;
  currentSample := 0.;
  /* Create array relative to buffer size */
  let dims = [|2 * bufferSize|];
  let ba = Genarray.create(float32, c_layout, dims);
  /* Fill the array on loop and write */
  while (playing^ === true) {
    fill_ba(ba, setStep);
    Portaudio.write_stream_ba(stream, ba, 0, bufferSize);
  };
  ();
};