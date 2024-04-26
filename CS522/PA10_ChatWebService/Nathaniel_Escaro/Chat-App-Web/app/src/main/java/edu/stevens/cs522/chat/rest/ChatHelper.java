package edu.stevens.cs522.chat.rest;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

import edu.stevens.cs522.base.DateUtils;
import edu.stevens.cs522.base.work.OneTimeWorkRequest;
import edu.stevens.cs522.base.work.PeriodicWorkRequest;
import edu.stevens.cs522.base.work.WorkManager;
import edu.stevens.cs522.chat.entities.Message;
import edu.stevens.cs522.chat.location.CurrentLocation;
import edu.stevens.cs522.chat.rest.work.PostMessageWorker;
import edu.stevens.cs522.chat.rest.work.SynchronizeWorker;
import edu.stevens.cs522.chat.services.RegisterService;
import edu.stevens.cs522.chat.settings.Settings;


/**
 * Created by dduggan.
 */

public class ChatHelper {

    private static final String TAG = ChatHelper.class.getCanonicalName();

    public static final int SYNC_INTERVAL = 1;  // 5

    private final Context context;

    private final WorkManager workManager;

    private final CurrentLocation location;

    public ChatHelper(Context context) {
        this.context = context;
        this.workManager = WorkManager.getInstance(context);
        this.location = new CurrentLocation(context);
    }

    public void register (Uri chatServer, String chatName) {
        if (chatName != null && !chatName.isEmpty()) {
            RegisterService.register(context, chatServer, chatName);
        }
    }

    public void postMessage(String chatRoom, String messageText) {
        if (messageText != null && !messageText.isEmpty()) {
            Log.d(TAG, "Posting message: "+messageText);
            Message mesg = new Message();
            mesg.messageText = messageText;
            mesg.appID = Settings.getAppId(context);
            mesg.chatroom = chatRoom;
            mesg.timestamp = DateUtils.now();
            mesg.latitude = location.getLatitude();
            mesg.longitude = location.getLongitude();
            mesg.sender = Settings.getChatName(context);

            Bundle data = new Bundle();
            data.putParcelable(PostMessageWorker.MESSAGE_KEY, mesg);

            /*
             * Depending on Settings.SYNC, message will be sent immediately, or just added locally
             * and eventually synchronized with server database.  The request processor
             * is where either of these will be done.
            */
            OneTimeWorkRequest workRequest = new OneTimeWorkRequest(PostMessageWorker.class, data);
            workManager.enqueueUniqueWork(workRequest);
        }
    }

    private PeriodicWorkRequest syncRequest;

    public void startMessageSync() {
        if (Settings.SYNC) {
            Log.d(TAG, "Enabling background synchronization of message database.");

            if (syncRequest != null) {
                throw new IllegalStateException("Trying to schedule sync when it is already scheduled!");
            }

            syncRequest = new PeriodicWorkRequest(SynchronizeWorker.class, null, SYNC_INTERVAL);
            workManager.enqueuePeriodicUniqueWork(syncRequest);
        }
    }

    public void stopMessageSync() {
        if (Settings.SYNC) {
            Log.d(TAG, "Canceling background synchronization of message database.");

            if (syncRequest == null) {
                throw new IllegalStateException("Trying to cancel sync when it is not scheduled!");
            }

            workManager.cancelPeriodicUniqueWork(syncRequest);
            syncRequest = null;
        }
    }

}
