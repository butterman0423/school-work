package edu.stevens.cs522.hello;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class ShowActivity extends Activity {
    public final static String MESSAGE_KEY = "message";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_show);

        // Child is created by Intent, so getIntent() -> non null val
        String input = getIntent().getStringExtra(MESSAGE_KEY);
        TextView txtView = (TextView) findViewById(R.id.message);

        String message = getResources().getString(R.string.show_name, input);
        txtView.setText(message);
    }
}