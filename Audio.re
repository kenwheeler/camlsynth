open Portaudio;
open Bigarray;
open Settings;
let leftPhase = ref(0);
let rightPhase = ref(0);
let leftInc = ref(1);
let rightInc = ref(1);

Random.self_init();
Portaudio.init();

let osc2 = Osc.create();

let fill_ba = ba => {
  for (i in 0 to bufferSize - 1) {
    /* osc#generate(arr, bufferSize); */
    let leftVal = osc2.table[leftPhase^];
    let rightVal = osc2.table[rightPhase^];

    let left = [|2 * i|];
    let right = [|2 * i + 1|];

    Genarray.set(ba, left, leftVal);
    Genarray.set(ba, right, rightVal);

    leftPhase := leftPhase^ + leftInc^;
    if (leftPhase^ >= tableSize) {
      leftPhase := leftPhase^ - tableSize;
    };
    rightPhase := rightPhase^ + rightInc^;
    if (rightPhase^ >= tableSize) {
      rightPhase := rightPhase^ - tableSize;
    };
  };
};

let test_bigarray = (stream, inter, batype) => {
  let bufferCount = 1 * int_of_float(sampleRate) / bufferSize;
  let dims = [|2 * bufferSize|];
  let ba = Genarray.create(batype, c_layout, dims);
  for (j in 0 to bufferCount - 1) {
    fill_ba(ba);
    Portaudio.write_stream_ba(stream, ba, 0, bufferSize);
  };
};

let deviceId = Portaudio.get_default_output_device();
let device = Portaudio.get_device_info(deviceId);

let play = () => {
  let outparam =
    Some({
      channels: 2,
      device: deviceId,
      sample_format: format_float32,
      latency: device.d_default_low_output_latency,
    });

  let stream =
    open_stream(None, outparam, device.d_default_sample_rate, bufferSize, []);

  start_stream(stream);
  test_bigarray(stream, 1, float32);
  close_stream(stream);
};