#include <Arduino.h>
#include <RTClib.h>

#define WavSerial Serial1

RTC_DS3231 rtc;

const int ACTIVE_HOUR_START = 10;
const int ACTIVE_HOUR_END = 23;

// Mode switch
const int MODE_SWITCH_PIN = 26;  
bool testMode = false;
bool lastModeSwitchState = HIGH;

// Audio segments
const int TOTAL_SEGMENTS = 213;
const int TRACK_PERFORMANCE_MODE = 214;
const int TRACK_TEST_MODE = 215;

// Segment durations in seconds (generated from split files)
const float SEGMENT_DURATIONS[TOTAL_SEGMENTS] = {
  0.626939, // Segment 1
  256.859138, // Segment 2
  6.896327, // Segment 3
  6.942766, // Segment 4
  25.797370, // Segment 5
  5.294150, // Segment 6
  383.106032, // Segment 7
  205.798458, // Segment 8
  32.995556, // Segment 9
  0.510839, // Segment 10
  372.401633, // Segment 11
  0.394739, // Segment 12
  2.995374, // Segment 13
  0.487619, // Segment 14
  7.105306, // Segment 15
  404.793469, // Segment 16
  205.798458, // Segment 17
  5.688889, // Segment 18
  16.857687, // Segment 19
  15.952109, // Segment 20
  77.554649, // Segment 21
  0.580499, // Segment 22
  270.930431, // Segment 23
  1.207438, // Segment 24
  1.184218, // Segment 25
  5.572789, // Segment 26
  24.357732, // Segment 27
  15.209070, // Segment 28
  26.099229, // Segment 29
  7.685805, // Segment 30
  112.454240, // Segment 31
  7.941224, // Segment 32
  0.882358, // Segment 33
  40.472381, // Segment 34
  14.651791, // Segment 35
  39.543583, // Segment 36
  13.606893, // Segment 37
  1.091338, // Segment 38
  35.480091, // Segment 39
  0.789478, // Segment 40
  9.125442, // Segment 41
  32.089977, // Segment 42
  7.709025, // Segment 43
  6.803447, // Segment 44
  22.453696, // Segment 45
  0.743039, // Segment 46
  6.571247, // Segment 47
  9.102222, // Segment 48
  16.950567, // Segment 49
  0.487619, // Segment 50
  1.160998, // Segment 51
  2.113016, // Segment 52
  188.290612, // Segment 53
  6.873107, // Segment 54
  6.965986, // Segment 55
  25.797370, // Segment 56
  5.294150, // Segment 57
  383.106032, // Segment 58
  238.794014, // Segment 59
  0.510839, // Segment 60
  200.156009, // Segment 61
  26.261769, // Segment 62
  298.144218, // Segment 63
  5.224490, // Segment 64
  3.436553, // Segment 65
  7.314286, // Segment 66
  12.678095, // Segment 67
  392.115374, // Segment 68
  7.941224, // Segment 69
  22.546576, // Segment 70
  15.928889, // Segment 71
  32.809796, // Segment 72
  7.662585, // Segment 73
  308.616417, // Segment 74
  1.207438, // Segment 75
  1.160998, // Segment 76
  5.572789, // Segment 77
  24.380952, // Segment 78
  15.209070, // Segment 79
  26.099229, // Segment 80
  7.662585, // Segment 81
  61.718639, // Segment 82
  8.753923, // Segment 83
  4.876190, // Segment 84
  86.424671, // Segment 85
  247.849796, // Segment 86
  23.336054, // Segment 87
  1.253878, // Segment 88
  31.950658, // Segment 89
  43.676735, // Segment 90
  48.994104, // Segment 91
  399.894059, // Segment 92
  16.230748, // Segment 93
  180.790567, // Segment 94
  1.300317, // Segment 95
  8.057324, // Segment 96
  8.962902, // Segment 97
  423.949932, // Segment 98
  6.339048, // Segment 99
  8.335964, // Segment 100
  8.986122, // Segment 101
  9.427302, // Segment 102
  1.184218, // Segment 103
  424.158912, // Segment 104
  232.780045, // Segment 105
  14.117732, // Segment 106
  6.733787, // Segment 107
  8.150204, // Segment 108
  8.544943, // Segment 109
  15.441270, // Segment 110
  4.969070, // Segment 111
  25.658050, // Segment 112
  148.398730, // Segment 113
  4.063492, // Segment 114
  461.473379, // Segment 115
  58.049887, // Segment 116
  9.171882, // Segment 117
  5.688889, // Segment 118
  11.308118, // Segment 119
  4.643991, // Segment 120
  385.938866, // Segment 121
  16.950567, // Segment 122
  32.902676, // Segment 123
  7.244626, // Segment 124
  30.371701, // Segment 125
  66.014331, // Segment 126
  338.152200, // Segment 127
  8.498503, // Segment 128
  8.521723, // Segment 129
  350.435556, // Segment 130
  0.696599, // Segment 131
  16.207528, // Segment 132
  27.353107, // Segment 133
  1.880816, // Segment 134
  12.840635, // Segment 135
  34.528073, // Segment 136
  31.207619, // Segment 137
  0.580499, // Segment 138
  0.441179, // Segment 139
  3.506213, // Segment 140
  205.380499, // Segment 141
  1.996916, // Segment 142
  7.523265, // Segment 143
  14.628571, // Segment 144
  7.685805, // Segment 145
  437.324626, // Segment 146
  17.229206, // Segment 147
  396.666485, // Segment 148
  0.696599, // Segment 149
  9.241542, // Segment 150
  33.297415, // Segment 151
  15.719909, // Segment 152
  1.834376, // Segment 153
  39.566803, // Segment 154
  7.035646, // Segment 155
  308.662857, // Segment 156
  26.981587, // Segment 157
  295.148844, // Segment 158
  223.515283, // Segment 159
  187.895873, // Segment 160
  7.151746, // Segment 161
  1.346757, // Segment 162
  8.405624, // Segment 163
  7.592925, // Segment 164
  208.561633, // Segment 165
  35.247891, // Segment 166
  2.089796, // Segment 167
  70.960181, // Segment 168
  6.849887, // Segment 169
  83.591837, // Segment 170
  16.997007, // Segment 171
  5.503129, // Segment 172
  7.360726, // Segment 173
  212.439365, // Segment 174
  225.999819, // Segment 175
  7.755465, // Segment 176
  6.153288, // Segment 177
  0.789478, // Segment 178
  0.696599, // Segment 179
  415.753288, // Segment 180
  30.812880, // Segment 181
  202.594104, // Segment 182
  9.102222, // Segment 183
  39.938322, // Segment 184
  6.942766, // Segment 185
  233.360544, // Segment 186
  0.743039, // Segment 187
  45.000272, // Segment 188
  7.105306, // Segment 189
  92.043900, // Segment 190
  11.656417, // Segment 191
  6.292608, // Segment 192
  5.619229, // Segment 193
  15.534150, // Segment 194
  276.967619, // Segment 195
  7.778685, // Segment 196
  1.160998, // Segment 197
  21.106939, // Segment 198
  8.382404, // Segment 199
  57.655147, // Segment 200
  20.410340, // Segment 201
  25.402630, // Segment 202
  6.246168, // Segment 203
  258.554195, // Segment 204
  58.746485, // Segment 205
  22.198277, // Segment 206
  14.187392, // Segment 207
  2.159456, // Segment 208
  1.160998, // Segment 209
  10.634739, // Segment 210
  31.300499, // Segment 211
  311.704671, // Segment 212
  8.235805  // Segment 213
};
// Total duration: 14719.75 seconds (245.33 minutes)

// Volume settings
const int VOLUME_NORMAL = -20;  // dB
const int VOLUME_MUTED = -70;   // dB

// Playback state
int currentSegment = 0;
bool isPlaying = false;
unsigned long segmentStartTime = 0;  // When current segment started playing
unsigned long lastTimeCheck = 0;
const unsigned long TIME_CHECK_INTERVAL = 60000;  // Check every minute

// Sync state
bool isPlayingFiller = false;  // True if playing filler segment waiting for sync
unsigned long syncMoment = 0;  // millis() when we should start the correct segment
int targetSegment = 0;  // The segment we should start at sync moment

// Fade state for 23:00 transition
bool isFading = false;
unsigned long fadeStartTime = 0;
const unsigned long FADE_DURATION = 10000;  // 10 second fade at 23:00

// Track reporting
bool trackReportingEnabled = false;

// Function declarations
void triggerTrack(int trackNum);
void stopTrack(int trackNum);
void setMasterVolume(int gain);
void trackLoop(int trackNum, bool enable);
void trackFade(int trackNum, int targetGain, int timeMs, bool stopFlag);
void setReporting(bool enable);
void checkWavTriggerSerial();
int calculateCurrentSegment();
int findFillerSegment(float minDuration);
void startPlaybackFromSegment(int segment);
void startSyncedPlayback();
void playNextSegment();
void handlePlayback();
void checkModeSwitch();
void printCurrentTime();
bool isActiveHours();

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Uncomment once to set time

  DateTime now = rtc.now();
  Serial.print("RTC time: ");
  Serial.print(now.year());
  Serial.print("-");
  if (now.month() < 10) Serial.print("0");
  Serial.print(now.month());
  Serial.print("-");
  if (now.day() < 10) Serial.print("0");
  Serial.print(now.day());
  Serial.print(" ");
  if (now.hour() < 10) Serial.print("0");
  Serial.print(now.hour());
  Serial.print(":");
  if (now.minute() < 10) Serial.print("0");
  Serial.println(now.minute());

  Serial.println("Continuous looping audio player starting...");
  Serial.print("Total segments: ");
  Serial.println(TOTAL_SEGMENTS);
  Serial.println("Daily offset algorithm: (day_of_year * 97) % 213");
  
  // Setup mode switch with internal pull-up
  pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);
  lastModeSwitchState = digitalRead(MODE_SWITCH_PIN);
  testMode = (lastModeSwitchState == LOW);  // LOW = test mode, HIGH = performance mode
  Serial.print("Starting in ");
  Serial.println(testMode ? "TEST MODE" : "PERFORMANCE MODE");
  
  Serial.println("Starting Wav Trigger serial...");
  WavSerial.begin(57600);
  delay(500);
  
  // Enable track reporting so we know when tracks end
  Serial.println("Enabling track reporting...");
  setReporting(true);
  delay(100);
  
  // Calculate which segment we should be playing
  currentSegment = calculateCurrentSegment();
  
  Serial.print("Calculated starting segment: ");
  Serial.println(currentSegment);
  
  // Start synced playback (may use filler if mid-segment)
  startSyncedPlayback();
  
  lastTimeCheck = millis();
  
  Serial.println("Ready!");
}

void loop() {
  // Check mode switch
  checkModeSwitch();
  
  // Check if we've reached the sync moment (when playing filler)
  if (isPlayingFiller && millis() >= syncMoment) {
    Serial.println("=== SYNC MOMENT REACHED ===");
    
    // Fade out filler quickly (1 second fade)
    trackFade(currentSegment, VOLUME_MUTED, 1000, true);
    delay(1100);  // Wait for fade to complete
    
    // Start the correct segment
    isPlayingFiller = false;
    currentSegment = targetSegment;
    segmentStartTime = millis();
    
    // Set correct volume
    if (testMode || isActiveHours()) {
      setMasterVolume(VOLUME_NORMAL);
    } else {
      setMasterVolume(VOLUME_MUTED);
    }
    
    triggerTrack(currentSegment);
    isPlaying = true;
    
    Serial.print("Now playing correct segment: ");
    Serial.print(currentSegment);
    Serial.print(" (duration: ");
    Serial.print(SEGMENT_DURATIONS[currentSegment - 1]);
    Serial.println("s)");
  }
  
  // Check if current segment should have ended based on duration (only if not playing filler)
  if (isPlaying && !isPlayingFiller && currentSegment > 0) {
    unsigned long elapsedMs = millis() - segmentStartTime;
    unsigned long segmentDurationMs = (unsigned long)(SEGMENT_DURATIONS[currentSegment - 1] * 1000);
    
    // Add 100ms buffer to account for timing variations
    if (elapsedMs >= segmentDurationMs + 100) {
      Serial.print("Segment ");
      Serial.print(currentSegment);
      Serial.print(" duration exceeded (");
      Serial.print(elapsedMs / 1000.0);
      Serial.print("s / ");
      Serial.print(segmentDurationMs / 1000.0);
      Serial.println("s) - playing next");
      playNextSegment();
    }
  }
  
  // Check for messages from WAV Trigger (for debugging, not critical)
  checkWavTriggerSerial();
  
  // Print time every minute
  if (millis() - lastTimeCheck >= TIME_CHECK_INTERVAL) {
    printCurrentTime();
    lastTimeCheck = millis();
  }
  
  // Handle playback and volume based on active hours
  handlePlayback();
  
  delay(10);  // Small delay to prevent tight loop
}

int calculateCurrentSegment() {
  DateTime now = rtc.now();
  
  // Calculate day of year (1-365/366)
  int dayOfYear = now.day();
  int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  // Check for leap year
  int year = now.year();
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    daysInMonth[2] = 29;
  }
  
  for (int i = 1; i < now.month(); i++) {
    dayOfYear += daysInMonth[i];
  }
  
  Serial.print("Day of year: ");
  Serial.println(dayOfYear);
  
  // Calculate daily starting segment using prime number 97
  int dailyStartSegment = (dayOfYear * 97) % TOTAL_SEGMENTS;
  if (dailyStartSegment == 0) dailyStartSegment = TOTAL_SEGMENTS;  // 1-indexed
  
  Serial.print("Daily start segment: ");
  Serial.println(dailyStartSegment);
  
  // If in test mode, just start from daily offset
  if (testMode) {
    Serial.println("Test mode: starting from daily offset");
    return dailyStartSegment;
  }
  
  // In performance mode, calculate exact position based on time
  int currentHour = now.hour();
  int currentMinute = now.minute();
  int currentSecond = now.second();
  
  // If before 10:00, playback hasn't started yet - return daily start
  if (currentHour < ACTIVE_HOUR_START) {
    Serial.println("Before active hours: will start at 10:00");
    return dailyStartSegment;
  }
  
  // Calculate seconds since 10:00
  int secondsSince10am = ((currentHour - ACTIVE_HOUR_START) * 3600) + 
                          (currentMinute * 60) + 
                          currentSecond;
  
  Serial.print("Seconds since 10:00: ");
  Serial.println(secondsSince10am);
  
  // Calculate which segment we should be on
  // Start from dailyStartSegment and add up durations
  float elapsedTime = 0;
  int segmentIndex = dailyStartSegment - 1;  // Convert to 0-indexed
  
  // Sum up segment durations until we exceed secondsSince10am
  while (elapsedTime < secondsSince10am) {
    elapsedTime += SEGMENT_DURATIONS[segmentIndex];
    
    if (elapsedTime < secondsSince10am) {
      segmentIndex++;
      
      // Wrap around if we go past the end
      if (segmentIndex >= TOTAL_SEGMENTS) {
        segmentIndex = 0;
      }
    }
  }
  
  int currentSegment = segmentIndex + 1;  // Convert back to 1-indexed
  
  Serial.print("Calculated current segment: ");
  Serial.print(currentSegment);
  Serial.print(" (elapsed time through segments: ");
  Serial.print(elapsedTime);
  Serial.println("s)");
  
  return currentSegment;
}

void startPlaybackFromSegment(int segment) {
  Serial.print("Starting playback from segment ");
  Serial.println(segment);
  
  currentSegment = segment;
  segmentStartTime = millis();  // Record when this segment started
  
  // Set initial volume based on mode and time
  if (testMode || isActiveHours()) {
    setMasterVolume(VOLUME_NORMAL);
    Serial.println("Volume: NORMAL");
  } else {
    setMasterVolume(VOLUME_MUTED);
    Serial.println("Volume: MUTED");
  }
  
  // Start playing (don't enable looping - we'll play sequentially)
  triggerTrack(currentSegment);
  isPlaying = true;
  
  Serial.print("Now playing segment: ");
  Serial.print(currentSegment);
  Serial.print(" (duration: ");
  Serial.print(SEGMENT_DURATIONS[currentSegment - 1]);
  Serial.println("s)");
}

void startSyncedPlayback() {
  DateTime now = rtc.now();
  
  // If in test mode, just start immediately
  if (testMode) {
    Serial.println("Test mode: starting immediately");
    startPlaybackFromSegment(currentSegment);
    return;
  }
  
  // Calculate how far into the current segment we should be
  int currentHour = now.hour();
  int currentMinute = now.minute();
  int currentSecond = now.second();
  
  // If before active hours, just start from the daily offset
  if (currentHour < ACTIVE_HOUR_START) {
    Serial.println("Before active hours: starting from daily offset");
    startPlaybackFromSegment(currentSegment);
    return;
  }
  
  // Calculate seconds since 10:00
  int secondsSince10am = ((currentHour - ACTIVE_HOUR_START) * 3600) + 
                          (currentMinute * 60) + 
                          currentSecond;
  
  // Calculate day of year for daily offset
  int dayOfYear = now.day();
  int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int year = now.year();
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    daysInMonth[2] = 29;
  }
  for (int i = 1; i < now.month(); i++) {
    dayOfYear += daysInMonth[i];
  }
  
  int dailyStartSegment = (dayOfYear * 97) % TOTAL_SEGMENTS;
  if (dailyStartSegment == 0) dailyStartSegment = TOTAL_SEGMENTS;
  
  // Sum up durations to find where we are
  float elapsedTime = 0;
  int segmentIndex = dailyStartSegment - 1;
  
  while (elapsedTime < secondsSince10am) {
    float segmentDuration = SEGMENT_DURATIONS[segmentIndex];
    
    if (elapsedTime + segmentDuration < secondsSince10am) {
      // This segment has completely elapsed
      elapsedTime += segmentDuration;
      segmentIndex++;
      if (segmentIndex >= TOTAL_SEGMENTS) segmentIndex = 0;
    } else {
      // We're partway through this segment
      float timeIntoSegment = secondsSince10am - elapsedTime;
      float timeRemaining = segmentDuration - timeIntoSegment;
      
      Serial.print("Mid-segment: ");
      Serial.print(timeIntoSegment);
      Serial.print("s into segment ");
      Serial.print(segmentIndex + 1);
      Serial.print(", ");
      Serial.print(timeRemaining);
      Serial.println("s remaining");
      
      // Find a filler segment >= timeRemaining
      int fillerSegment = findFillerSegment(timeRemaining);
      
      if (fillerSegment > 0) {
        Serial.print("Using filler segment ");
        Serial.print(fillerSegment);
        Serial.print(" (duration: ");
        Serial.print(SEGMENT_DURATIONS[fillerSegment - 1]);
        Serial.println("s)");
        
        // Play filler
        isPlayingFiller = true;
        currentSegment = fillerSegment;
        targetSegment = segmentIndex + 2;  // Next segment after current
        if (targetSegment > TOTAL_SEGMENTS) targetSegment = 1;
        
        syncMoment = millis() + (unsigned long)(timeRemaining * 1000);
        
        if (testMode || isActiveHours()) {
          setMasterVolume(VOLUME_NORMAL);
        } else {
          setMasterVolume(VOLUME_MUTED);
        }
        
        triggerTrack(currentSegment);
        isPlaying = true;
        
        Serial.print("Playing filler, will sync to segment ");
        Serial.print(targetSegment);
        Serial.print(" in ");
        Serial.print(timeRemaining);
        Serial.println("s");
      } else {
        // No suitable filler, just wait in silence
        Serial.println("No suitable filler found, waiting in silence");
        delay((unsigned long)(timeRemaining * 1000));
        
        // Start next segment
        currentSegment = segmentIndex + 2;
        if (currentSegment > TOTAL_SEGMENTS) currentSegment = 1;
        startPlaybackFromSegment(currentSegment);
      }
      
      return;
    }
  }
  
  // If we get here, we're at a segment boundary - perfect!
  Serial.println("At segment boundary - starting immediately");
  currentSegment = segmentIndex + 1;
  if (currentSegment > TOTAL_SEGMENTS) currentSegment = 1;
  startPlaybackFromSegment(currentSegment);
}

int findFillerSegment(float minDuration) {
  // Find the shortest segment that's >= minDuration
  int bestSegment = -1;
  float bestDuration = 999999;
  
  for (int i = 0; i < TOTAL_SEGMENTS; i++) {
    float duration = SEGMENT_DURATIONS[i];
    if (duration >= minDuration && duration < bestDuration) {
      bestDuration = duration;
      bestSegment = i + 1;  // Convert to 1-indexed
    }
  }
  
  return bestSegment;
}

void handlePlayback() {
  DateTime now = rtc.now();
  bool shouldBeAudible = testMode || isActiveHours();
  
  // Handle volume transitions
  static bool wasAudible = shouldBeAudible;
  
  if (shouldBeAudible != wasAudible) {
    if (shouldBeAudible) {
      // Entering active hours - fade in
      Serial.println("=== ENTERING ACTIVE HOURS - FADING IN ===");
      setMasterVolume(VOLUME_NORMAL);
    } else {
      // Leaving active hours - fade out at 23:00
      if (now.hour() == ACTIVE_HOUR_END && !isFading) {
        Serial.println("=== LEAVING ACTIVE HOURS - FADING OUT ===");
        isFading = true;
        fadeStartTime = millis();
        
        // Start fade out over 10 seconds
        if (isPlaying) {
          trackFade(currentSegment, VOLUME_MUTED, FADE_DURATION, false);
        }
      }
    }
    wasAudible = shouldBeAudible;
  }
  
  // Check if fade is complete
  if (isFading && (millis() - fadeStartTime >= FADE_DURATION)) {
    isFading = false;
    Serial.println("Fade complete - now muted");
  }
  
  // Sequential playback is handled by checkWavTriggerSerial() 
  // which listens for track end notifications
}

void checkWavTriggerSerial() {
  // WAV Trigger track reporting message format:
  // 0xF0 0xAA <length> 0x81 <track_low> <track_high> <voice> <state> 0x55
  // state: 1 = started, 0 = stopped
  
  static byte msgBuffer[10];
  static int msgIndex = 0;
  
  while (WavSerial.available()) {
    byte b = WavSerial.read();
    
    // Look for start of message
    if (msgIndex == 0 && b == 0xF0) {
      msgBuffer[msgIndex++] = b;
    }
    else if (msgIndex == 1 && b == 0xAA) {
      msgBuffer[msgIndex++] = b;
    }
    else if (msgIndex >= 2 && msgIndex < 10) {
      msgBuffer[msgIndex++] = b;
      
      // Check if we have a complete message (ends with 0x55)
      if (b == 0x55) {
        // Parse the message
        if (msgBuffer[3] == 0x81) {  // Track report message
          int trackNum = msgBuffer[4] | (msgBuffer[5] << 8);
          byte voice = msgBuffer[6];
          byte state = msgBuffer[7];
          
          if (state == 0) {  // Track stopped
            Serial.print("Track ");
            Serial.print(trackNum);
            Serial.println(" ended - playing next segment");
            playNextSegment();
          }
        }
        msgIndex = 0;  // Reset for next message
      }
    }
    else {
      msgIndex = 0;  // Reset on invalid sequence
    }
  }
}

void playNextSegment() {
  // Advance to next segment
  currentSegment++;
  
  // Wrap around to segment 1 after 213
  if (currentSegment > TOTAL_SEGMENTS) {
    currentSegment = 1;
    Serial.println("=== WRAPPED TO SEGMENT 1 ===");
  }
  
  // Record start time for this segment
  segmentStartTime = millis();
  
  // Play the next segment
  triggerTrack(currentSegment);
  
  Serial.print("Now playing segment: ");
  Serial.print(currentSegment);
  Serial.print(" (duration: ");
  Serial.print(SEGMENT_DURATIONS[currentSegment - 1]);
  Serial.println("s)");
}

void setReporting(bool enable) {
  // Enable/disable track reporting
  WavSerial.write((byte)0xF0);           // SOM1
  WavSerial.write((byte)0xAA);           // SOM2
  WavSerial.write((byte)0x06);           // Message length (6 bytes)
  WavSerial.write((byte)0x50);           // Command: SET_REPORTING
  WavSerial.write((byte)(enable ? 0x01 : 0x00)); // Enable/disable
  WavSerial.write((byte)0x55);           // EOM
}

void printCurrentTime() {
  DateTime now = rtc.now();
  Serial.print("Time check: ");
  if (now.hour() < 10) Serial.print("0");
  Serial.print(now.hour());
  Serial.print(":");
  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute());
  Serial.print(" | Segment: ");
  Serial.print(currentSegment);
  Serial.print(" | Mode: ");
  Serial.print(testMode ? "TEST" : "PERF");
  Serial.print(" | Volume: ");
  Serial.println((testMode || isActiveHours()) ? "NORMAL" : "MUTED");
}

bool isActiveHours() {
  DateTime now = rtc.now();
  int hour = now.hour();
  return hour >= ACTIVE_HOUR_START && hour < ACTIVE_HOUR_END;
}

void triggerTrack(int trackNum) {
  // PLAY_SOLO mode: stops other tracks and plays this one
  WavSerial.write((byte)0xF0);           // SOM1
  WavSerial.write((byte)0xAA);           // SOM2
  WavSerial.write((byte)0x08);           // Message length (8 bytes total)
  WavSerial.write((byte)0x03);           // Command: CONTROL_TRACK
  WavSerial.write((byte)0x00);           // Play code: 0x00 = PLAY_SOLO
  WavSerial.write((byte)trackNum);       // Track number low byte
  WavSerial.write((byte)(trackNum >> 8)); // Track number high byte
  WavSerial.write((byte)0x55);           // EOM
}

void stopTrack(int trackNum) {
  WavSerial.write((byte)0xF0);           // SOM1
  WavSerial.write((byte)0xAA);           // SOM2
  WavSerial.write((byte)0x08);           // Message length (8 bytes total)
  WavSerial.write((byte)0x03);           // Command: CONTROL_TRACK
  WavSerial.write((byte)0x04);           // Play code: 0x04 = STOP
  WavSerial.write((byte)trackNum);       // Track number low byte
  WavSerial.write((byte)(trackNum >> 8)); // Track number high byte
  WavSerial.write((byte)0x55);           // EOM
}

void setMasterVolume(int gain) {
  // gain range: -70dB to +10dB
  // Sent as signed 16-bit integer, little-endian
  WavSerial.write((byte)0xF0);           // SOM1
  WavSerial.write((byte)0xAA);           // SOM2
  WavSerial.write((byte)0x07);           // Message length (7 bytes)
  WavSerial.write((byte)0x05);           // Command: MASTER_VOLUME
  WavSerial.write((byte)gain);           // Gain low byte
  WavSerial.write((byte)(gain >> 8));    // Gain high byte
  WavSerial.write((byte)0x55);           // EOM
}

void trackLoop(int trackNum, bool enable) {
  // Enable or disable looping for a track
  WavSerial.write((byte)0xF0);           // SOM1
  WavSerial.write((byte)0xAA);           // SOM2
  WavSerial.write((byte)0x08);           // Message length (8 bytes)
  WavSerial.write((byte)0x03);           // Command: CONTROL_TRACK
  WavSerial.write((byte)(enable ? 0x05 : 0x06)); // 0x05 = LOOP_ON, 0x06 = LOOP_OFF
  WavSerial.write((byte)trackNum);       // Track number low byte
  WavSerial.write((byte)(trackNum >> 8)); // Track number high byte
  WavSerial.write((byte)0x55);           // EOM
}

void trackFade(int trackNum, int targetGain, int timeMs, bool stopFlag) {
  // Fade track to target gain over timeMs milliseconds
  // If stopFlag is true, stop the track after fade completes
  WavSerial.write((byte)0xF0);           // SOM1
  WavSerial.write((byte)0xAA);           // SOM2
  WavSerial.write((byte)0x0C);           // Message length (12 bytes)
  WavSerial.write((byte)0x0A);           // Command: TRACK_FADE
  WavSerial.write((byte)trackNum);       // Track number low byte
  WavSerial.write((byte)(trackNum >> 8)); // Track number high byte
  WavSerial.write((byte)targetGain);     // Target gain low byte
  WavSerial.write((byte)(targetGain >> 8)); // Target gain high byte
  WavSerial.write((byte)timeMs);         // Time low byte
  WavSerial.write((byte)(timeMs >> 8));  // Time high byte
  WavSerial.write((byte)(timeMs >> 16)); // Time byte 3
  WavSerial.write((byte)(timeMs >> 24)); // Time byte 4
  WavSerial.write((byte)(stopFlag ? 0x01 : 0x00)); // Stop flag
  WavSerial.write((byte)0x55);           // EOM
}

void checkModeSwitch() {
  int reading = digitalRead(MODE_SWITCH_PIN);
  
  // Check for state change
  if (reading != lastModeSwitchState) {
    delay(50);  // Simple debounce
    reading = digitalRead(MODE_SWITCH_PIN);
    
    if (reading != lastModeSwitchState) {
      lastModeSwitchState = reading;
      
      // Stop current playback
      if (isPlaying) {
        stopTrack(currentSegment);
        isPlaying = false;
        delay(100);
      }
      
      if (reading == LOW) {
        // Switched to TEST MODE
        testMode = true;
        Serial.println("=== SWITCHED TO TEST MODE ===");
        
        // Play test mode confirmation sound
        setMasterVolume(VOLUME_NORMAL);
        triggerTrack(TRACK_TEST_MODE);
        delay(2000);  // Wait for confirmation sound
        
        // Resume playback at normal volume
        startPlaybackFromSegment(currentSegment);
      } else {
        // Switched to PERFORMANCE MODE
        testMode = false;
        Serial.println("=== SWITCHED TO PERFORMANCE MODE ===");
        
        // Play performance mode confirmation sound
        setMasterVolume(VOLUME_NORMAL);
        triggerTrack(TRACK_PERFORMANCE_MODE);
        delay(2000);  // Wait for confirmation sound
        
        // Resume playback with appropriate volume
        startPlaybackFromSegment(currentSegment);
      }
    }
  }
}
