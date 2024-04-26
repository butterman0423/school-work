package edu.stevens.cs522.chat.activities;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import androidx.fragment.app.FragmentActivity;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.List;

import edu.stevens.cs522.chat.R;
import edu.stevens.cs522.chat.entities.Message;
import edu.stevens.cs522.chat.entities.Peer;
import edu.stevens.cs522.chat.ui.MessageChatroomAdapter;
import edu.stevens.cs522.chat.viewmodels.PeerViewModel;

/**
 * Created by dduggan.
 */

public class ViewPeerActivity extends FragmentActivity {

    public static final String TAG = ViewPeerActivity.class.getCanonicalName();

    public static final String PEER_KEY = "peer";

    private MessageChatroomAdapter messageAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.view_peer);

        Peer peer = getIntent().getParcelableExtra(PEER_KEY);
        if (peer == null) {
            throw new IllegalArgumentException("Expected peer id as intent extra");
        }

        TextView nameField = findViewById(R.id.view_user_name);
        TextView timeField = findViewById(R.id.view_timestamp);
        TextView locField = findViewById(R.id.view_location);

        nameField.setText( getString(R.string.view_user_name, peer.name) );
        timeField.setText( getString(R.string.view_timestamp, formatTimestamp(peer.timestamp)) );
        locField.setText( getString(R.string.view_location, peer.latitude, peer.longitude) );

        // Initialize the recyclerview and adapter for messages
        RecyclerView messageList = findViewById(R.id.message_list);
        messageList.setLayoutManager(new LinearLayoutManager(this));

        messageAdapter = new MessageChatroomAdapter();
        messageList.setAdapter(messageAdapter);

        PeerViewModel peerViewModel = new ViewModelProvider(this).get(PeerViewModel.class);
        LiveData<List<Message>> liveMsgs = peerViewModel.fetchMessagesFromPeer(peer);

        liveMsgs.observe(this, msgs -> {
            messageAdapter.setMessages(msgs);
            messageAdapter.notifyDataSetChanged();
        });

    }

    private static String formatTimestamp(Date timestamp) {
        LocalDateTime dateTime = timestamp.toInstant().atZone(ZoneId.systemDefault()).toLocalDateTime();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd-MM-yyyy HH:mm:ss");
        return dateTime.format(formatter);
    }



}
