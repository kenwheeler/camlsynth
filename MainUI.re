open Revery;
open Revery.UI;
open Revery.UI.Components;
open Audio;
open Thread;
open Portaudio;
open AppState;

type threadArgs = {
  dispatch: action => unit,
  appStateRef: ref(appState),
};

let component = React.component("DrumMachine");

type streamParams =
  option(Portaudio.stream_parameters(float, Bigarray.float32_elt));

let stream:
  Portaudio.stream(streamParams, streamParams, float, Bigarray.float32_elt) =
  getStream();

let appStateRef = ref(initialAppState);

let createElement = (~children as _, ()) =>
  component(hooks => {
    let (appState, dispatch, hooks) =
      Hooks.reducer(~initialState=initialAppState, reducer, hooks);

    let hooks =
      Hooks.effect(
        Always,
        () => {
          appStateRef := appState;
          None;
        },
        hooks,
      );

    let setActiveStep = step => {
      dispatch(SetActiveStep(step - 1));
    };

    let updateTrack = (track, ()) => {
      dispatch(SetActiveTrack(track));
    };

    let updateTempo = (tempo, ()) => {
      dispatch(UpdateTempo(tempo));
    };

    let playToggle = () => {
      if (appState.playing) {
        setActiveStep(0);
        Audio.stop(stream);
        dispatch(SetPlaying(false));
      } else {
        start_stream(stream);
        let _handle =
          Thread.create(
            ({dispatch, appStateRef}) =>
              Audio.play(stream, dispatch, appStateRef),
            {dispatch, appStateRef},
          );
        dispatch(SetPlaying(true));
      };

      ();
    };

    let containerStyle =
      Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        bottom(0),
        top(0),
        left(0),
        right(0),
      ];

    let innerStyle =
      Style.[
        flexGrow(0),
        alignSelf(`Stretch),
        flexDirection(`Row),
        alignItems(`FlexEnd),
        backgroundColor(Color.hex("#888888")),
        margin(20),
        height(100),
        borderRadius(5.),
        border(~width=1, ~color=Color.hex("#888888")),
      ];

    let headerStyle =
      Style.[
        flexDirection(`Row),
        justifyContent(`FlexEnd),
        alignSelf(`Stretch),
        paddingTop(20),
        paddingRight(20),
        borderBottom(~width=1, ~color=Color.hex("#F16F20")),
      ];
    let textHeaderStyle =
      Style.[
        color(Color.hex("#F16F20")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(24),
      ];

    let subTextStyle =
      Style.[
        color(Color.hex("#F16F20")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(18),
        marginLeft(20),
        marginTop(6),
      ];

    let trackStyle =
      Style.[
        alignItems(`FlexStart),
        flexDirection(`Row),
        flexGrow(1),
        marginLeft(20),
        justifyContent(`FlexStart),
      ];

    (
      hooks,
      <View style=containerStyle>
        <View style=headerStyle>
          <View style=trackStyle>
            <TrackButton
              onClick={updateTrack(0)}
              active={appState.activeTrack == 0}
              text="KCK"
            />
            <TrackButton
              onClick={updateTrack(1)}
              active={appState.activeTrack == 1}
              text="SNR"
            />
            <TrackButton
              onClick={updateTrack(2)}
              active={appState.activeTrack == 2}
              text="HAT"
            />
            <TrackButton
              onClick={updateTrack(3)}
              active={appState.activeTrack == 3}
              text="CBL"
            />
            <TrackButton
              onClick={updateTrack(4)}
              active={appState.activeTrack == 4}
              text="CLV"
            />
          </View>
          <TempoControl onChange=updateTempo tempo={appState.tempo} />
          <Text style=textHeaderStyle text="Reason Composer" />
          <Text style=subTextStyle text="ML-808" />
        </View>
        <View style=innerStyle>
          <PlayButton playing={appState.playing} onClick=playToggle />
          <SequencerPanel appState parentDispatch=dispatch />
        </View>
        <TrackFx appState parentDispatch=dispatch />
        <MasterFx appState parentDispatch=dispatch />
      </View>,
    );
  });