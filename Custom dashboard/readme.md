# ThingSpeak API Setup Guide

## Configuration Changes Required

You'll need to replace several placeholder values in the code with your actual ThingSpeak channel information:

### 1. Channel ID Replacement
Replace `Your channelID` in these locations:
- Base URLs for charts
- API endpoints for data fetching
- Error code fetching URL

### 2. API Key Configuration
Replace `Your API key` in:
- Chart URLs parameters
- Data update endpoints
- Error code fetching endpoint

### 3. Field Numbers
Replace `Your field Number` with the actual field numbers in your ThingSpeak channel:
- Chart iframes (charts 1-7)
- Error code fetching URL

## Chart Configuration

The code sets up 7 charts with different purposes:
1. Chart 1 (field 5)
2. Chart 2 (field 4)
3. Chart 3 (field 6)
4. Chart 4 (field 2)
5. Chart 5 (field 1)
6. Chart 6 (field 7) - Error codes
7. Chart 7 (field 8) - Switch values

## Step-by-Step Setup Instructions

1. **Create ThingSpeak Account**
   - Sign up at ThingSpeak.com
   - Create a new channel
   - Configure 8 fields for your data

2. **Get Your Credentials**
   - Navigate to your channel
   - Copy your Channel ID from the channel URL
   - Generate or copy your API Write Key and Read Key

3. **Code Updates**
   Replace in these sections:
   ```javascript
   // Base URLs
   const baseUrla = 'https://thingspeak.com/channels/YOUR_CHANNEL_ID/charts/';
   const baseUrl = 'https://thingspeak.com/channels/YOUR_CHANNEL_ID/charts/';
   
   // API Configuration
   const apiKey = 'YOUR_API_KEY';
   const channelID = 'YOUR_CHANNEL_ID';
   
   // Error Code Fetching
   const url = 'https://api.thingspeak.com/channels/YOUR_CHANNEL_ID/fields/7/last.json?api_key=YOUR_API_KEY';
   ```

4. **Field Mapping**
   - Field 1: General data
   - Field 2: General data
   - Field 4: General data
   - Field 5: General data
   - Field 6: General data
   - Field 7: Error codes
   - Field 8: Switch state

## Important Notes

1. The dashboard updates every 16 seconds (configurable in the setInterval calls)
2. The system includes retry logic for failed API calls with exponential backoff
3. Error codes are mapped as follows:
   - 0: System OK
   - 1: No Power
   - 2: Low Voltage
   - 3: High Current
   - 5: Prolonged High Current
   - 9: Short Circuit

## Testing

After making all changes:
1. Open the dashboard in a browser
2. Check browser console for any API errors
3. Verify all charts are loading
4. Test the switch functionality
5. Verify error status updates

## Troubleshooting

If charts aren't loading:
1. Verify API key permissions (read/write access)
2. Check field numbers match your channel configuration
3. Confirm channel ID is correct
4. Check browser console for specific error messages