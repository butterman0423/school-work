package edu.stevens.cs522.hello;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Retrieve button and connect listener
        Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        EditText txtBox = (EditText) findViewById(R.id.textbox);
        String txt = txtBox.getText().toString();

        Intent intent = new Intent(this, ShowActivity.class);
        intent.putExtra(ShowActivity.MESSAGE_KEY, txt);
        startActivity(intent);
    }
}