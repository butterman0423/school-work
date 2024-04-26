package edu.stevens.cs522.chat.rest.work;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.NonNull;

import edu.stevens.cs522.base.work.Worker;
import edu.stevens.cs522.chat.entities.Message;
import edu.stevens.cs522.chat.rest.RequestProcessor;
import edu.stevens.cs522.chat.rest.request.ChatServiceResponse;
import edu.stevens.cs522.chat.rest.request.ErrorResponse;
import edu.stevens.cs522.chat.rest.request.PostMessageRequest;

public class PostMessageWorker extends Worker {

    private static final String TAG = PostMessageWorker.class.getCanonicalName();

    public static final String MESSAGE_KEY = "message";

    private final Message message;

    public PostMessageWorker(@NonNull Context context, @NonNull Bundle data) {
        super(context, data);

        message = data.getParcelable(MESSAGE_KEY);
        if (message == null) {
            throw new IllegalStateException("Missing message for post message worker!");
        }
    }

    @Override
    public boolean doWork() {

        PostMessageRequest postMessageRequest = new PostMessageRequest(message);

        RequestProcessor processor = RequestProcessor.getInstance(context);

        ChatServiceResponse response = processor.process(postMessageRequest);

        if (response instanceof ErrorResponse) {
            Log.d(TAG, String.format("Failed to send message ('%s'): %s", message.messageText, response.httpResponseMessage));
            return false;
        } else {
            Log.d(TAG, String.format("Message sent ('%s')!", message.messageText));
            return true;
        }

    }
}
