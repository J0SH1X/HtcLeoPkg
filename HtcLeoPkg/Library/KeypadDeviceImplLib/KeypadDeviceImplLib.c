#include <PiDxe.h>

#include <Library/LKEnvLib.h>

#include <Library/KeypadDeviceHelperLib.h>
#include <Library/KeypadDeviceImplLib.h>
#include <Protocol/KeypadDevice.h>

typedef enum {
  KEY_DEVICE_TYPE_UNKNOWN,
  KEY_DEVICE_TYPE_LEGACY,
  KEY_DEVICE_TYPE_KEYMATRIX,
  KEY_DEVICE_TYPE_TLMM,
  KEY_DEVICE_TYPE_PM8X41,
  KEY_DEVICE_TYPE_PM8X41_PON,
} KEY_DEVICE_TYPE;

typedef struct {
  KEY_CONTEXT     EfiKeyContext;
  BOOLEAN         IsValid;
  KEY_DEVICE_TYPE DeviceType;

  // gpio
  UINT8   Gpio;
  BOOLEAN ActiveLow;

  // gpio keymatrix

  UINT8 GpioIn;
  UINT8 GpioOut;

  // pon
  UINT32 PonType;
} KEY_CONTEXT_PRIVATE;

STATIC KEY_CONTEXT_PRIVATE KeyContextPower;
STATIC KEY_CONTEXT_PRIVATE KeyContextVolumeUp;
STATIC KEY_CONTEXT_PRIVATE KeyContextVolumeDown;
STATIC KEY_CONTEXT_PRIVATE KeyContextBack;
STATIC KEY_CONTEXT_PRIVATE KeyContextWindows;
STATIC KEY_CONTEXT_PRIVATE KeyContextHome;
STATIC KEY_CONTEXT_PRIVATE KeyContextDial;

STATIC KEY_CONTEXT_PRIVATE *KeyList[] = {
    &KeyContextPower,      &KeyContextVolumeUp,      &KeyContextVolumeDown,
    &KeyContextBack, &KeyContextWindows, &KeyContextHome,
    &KeyContextDial};

STATIC
VOID KeypadInitializeKeyContextPrivate(KEY_CONTEXT_PRIVATE *Context)
{
  Context->IsValid    = FALSE;
  Context->Gpio       = 0;
  Context->DeviceType = KEY_DEVICE_TYPE_UNKNOWN;
  Context->ActiveLow  = FALSE;
}

STATIC
KEY_CONTEXT_PRIVATE *KeypadKeyCodeToKeyContext(UINT32 KeyCode)
{
  if (KeyCode == 114)
    return &KeyContextVolumeDown;
  else if (KeyCode == 115)
    return &KeyContextVolumeUp;
  else if (KeyCode == 116)
    return &KeyContextPower;
  else if (KeyCode == 117)
    return &KeyContextBack;
  else if (KeyCode == 118)
    return &KeyContextWindows;
  else if (KeyCode == 119)
    return &KeyContextHome;
  else if (KeyCode == 120)
    return &KeyContextDial;
  else
    return NULL;
}

RETURN_STATUS
EFIAPI
KeypadDeviceImplConstructor(VOID)
{
  UINTN                Index;
  KEY_CONTEXT_PRIVATE *StaticContext;

  // Reset all keys
  for (Index = 0; Index < ARRAY_SIZE(KeyList); Index++) {
    KeypadInitializeKeyContextPrivate(KeyList[Index]);
  }

  // Configure keys

  // power button
  StaticContext             = KeypadKeyCodeToKeyContext(116);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_LEGACY;
  StaticContext->Gpio       = 94;
  StaticContext->ActiveLow  = 0x1 & 0x1;
  StaticContext->IsValid    = TRUE;

    // volume up button
  StaticContext             = KeypadKeyCodeToKeyContext(116);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_KEYMATRIX;
  StaticContext->GpioOut    = 33;
  StaticContext->GpioIn     = 42;
  StaticContext->ActiveLow  = 0x1 & 0x1;
  StaticContext->IsValid    = TRUE;

    // volume down button
  StaticContext             = KeypadKeyCodeToKeyContext(116);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_KEYMATRIX;
  StaticContext->GpioOut    = 33;
  StaticContext->GpioIn     = 41;
  StaticContext->ActiveLow  = 0x1 & 0x1;
  StaticContext->IsValid    = TRUE;

  // back button
  StaticContext             = KeypadKeyCodeToKeyContext(117);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_KEYMATRIX;
  StaticContext->GpioOut    = 31;
  StaticContext->GpioIn     = 42;
  StaticContext->ActiveLow  = 0x1 & 0x1;
  StaticContext->IsValid    = TRUE;

  // windows button
  StaticContext             = KeypadKeyCodeToKeyContext(118);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_KEYMATRIX;
  StaticContext->GpioOut    = 32;
  StaticContext->GpioIn     = 42;
  StaticContext->ActiveLow  = 0x1 & 0x1;
  StaticContext->IsValid    = TRUE;

    // home button
  StaticContext             = KeypadKeyCodeToKeyContext(119);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_KEYMATRIX;
  StaticContext->GpioOut    = 31;
  StaticContext->GpioIn     = 41;
  StaticContext->ActiveLow  = 0x1 & 0x1;
  StaticContext->IsValid    = TRUE;


    // dial button
  StaticContext             = KeypadKeyCodeToKeyContext(120);
  StaticContext->DeviceType = KEY_DEVICE_TYPE_KEYMATRIX;
  StaticContext->GpioOut    = 32;
  StaticContext->GpioIn     = 41;
  StaticContext->ActiveLow  = 0x1 & 0x1;
  StaticContext->IsValid    = TRUE;



  return RETURN_SUCCESS;
}

EFI_STATUS EFIAPI KeypadDeviceImplReset(KEYPAD_DEVICE_PROTOCOL *This)
{
  LibKeyInitializeKeyContext(&KeyContextPower.EfiKeyContext);
  KeyContextPower.EfiKeyContext.KeyData.Key.UnicodeChar = CHAR_CARRIAGE_RETURN;

  LibKeyInitializeKeyContext(&KeyContextVolumeUp.EfiKeyContext);
  KeyContextVolumeUp.EfiKeyContext.KeyData.Key.ScanCode = SCAN_UP;

  LibKeyInitializeKeyContext(&KeyContextVolumeDown.EfiKeyContext);
  KeyContextVolumeDown.EfiKeyContext.KeyData.Key.ScanCode = SCAN_DOWN;

  // ToDo: replace with different keys that are returned

  LibKeyInitializeKeyContext(&KeyContextBack.EfiKeyContext);
  KeyContextBack.EfiKeyContext.KeyData.Key.ScanCode = SCAN_DOWN;

  LibKeyInitializeKeyContext(&KeyContextWindows.EfiKeyContext);
  KeyContextWindows.EfiKeyContext.KeyData.Key.ScanCode = SCAN_DOWN;

  LibKeyInitializeKeyContext(&KeyContextHome.EfiKeyContext);
  KeyContextHome.EfiKeyContext.KeyData.Key.ScanCode = SCAN_DOWN;

  LibKeyInitializeKeyContext(&KeyContextDial.EfiKeyContext);
  KeyContextDial.EfiKeyContext.KeyData.Key.ScanCode = SCAN_DOWN;

  // Todo: end

  return EFI_SUCCESS;
}

extern void gpio_set(unsigned n, unsigned on);
extern int  gpio_get(unsigned n);

EFI_STATUS KeypadDeviceImplGetKeys(
    KEYPAD_DEVICE_PROTOCOL *This, KEYPAD_RETURN_API *KeypadReturnApi,
    UINT64 Delta)
{
  UINT8   GpioStatus;
  BOOLEAN IsPressed;
  UINTN   Index;
  DEBUG((EFI_D_ERROR, "KeypadDeviceImplGetKeys!\n"));

  for (Index = 0; Index < (sizeof(KeyList) / sizeof(KeyList[0])); Index++) {
    KEY_CONTEXT_PRIVATE *Context = KeyList[Index];

    // check if this is a valid key
    if (Context->IsValid == FALSE)
      continue;

    // get status
     if (Context->DeviceType == KEY_DEVICE_TYPE_LEGACY) {
      // impliement hd2 gpio shit here
      GpioStatus = gpio_get(Context->Gpio);
      DEBUG((EFI_D_ERROR, "LEGACY KEYTYPE GPIO STATUS: %d!\n", GpioStatus));
      gpio_set(48, 1);
    }
    else if (Context->DeviceType == KEY_DEVICE_TYPE_KEYMATRIX) {
      DEBUG((EFI_D_ERROR, "KEYMATRIX!\n"));
      gpio_set(Context->GpioOut, 0);
      GpioStatus = gpio_get(Context->GpioIn);
    }
    else {
      continue;
    }

    // update key status
    // 0000 ^0001 = 0001 = decimal 1
    IsPressed = (GpioStatus ? 1 : 0) ^ Context->ActiveLow;

    DEBUG((EFI_D_ERROR, "IsPressed var is: %d!\n", IsPressed));

    if (IsPressed) {
      // impl a timed callback here that enables gpio 48 for a few secs and then
      // the callback function should disable it again
    }

    if (Context->DeviceType == KEY_DEVICE_TYPE_KEYMATRIX) {
      gpio_set(Context->GpioOut, 1);
    }

    LibKeyUpdateKeyStatus(
        &Context->EfiKeyContext, KeypadReturnApi, IsPressed, Delta);
  }

  return EFI_SUCCESS;
}
