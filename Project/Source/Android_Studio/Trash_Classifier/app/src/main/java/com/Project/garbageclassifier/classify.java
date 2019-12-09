package com.Project.garbageclassifier;

import android.content.ContentValues;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;


import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import butterknife.ButterKnife;
import butterknife.OnClick;

public class classify extends AppCompatActivity {
    private ImageView selected_image;
    Bitmap bitmap;

    TextView mTvPrediction, mTvProbability, mTvTimeCost;

    private Classifier mClassifier;
    private Uri imageUri;
    public static final int REQUEST_IMAGE = 100;
    private Button classify_button;
    private boolean organic;
    MqttAndroidClient client;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        init();
        super.onCreate(savedInstanceState);
                setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        selected_image=findViewById(R.id.selected_image);
        setContentView(R.layout.activity_main);
        selected_image=findViewById(R.id.selected_image);
        Uri uri = (Uri) getIntent().getParcelableExtra("resID_uri");
        try {
            bitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), uri);
            selected_image.setImageBitmap(bitmap);
            // not sure why this happens, but without this the image appears on its side
            selected_image.setRotation(selected_image.getRotation() + 90);
        } catch (
                IOException e) {
            e.printStackTrace();
        }
        classify_button = (Button)findViewById(R.id.btn_detect);
        classify_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mClassifier == null) {
                    return;
                }
                Bitmap image = Bitmap.createScaledBitmap(bitmap, 224, 224, true);
                Result result = mClassifier.classify(image);
                mTvPrediction = (TextView) findViewById(R.id.tv_prediction);
                if(result.getNumber() == 0) {
                    mTvPrediction.setText("Organic Bin");
                    organic = true;
                }
                else
                    mTvPrediction.setText("Recycle Bin");
                mTvProbability = (TextView) findViewById(R.id.tv_probability);
                mTvProbability.setText(String.valueOf(result.getProbability()));
                mTvTimeCost = (TextView) findViewById(R.id.tv_timecost);
                mTvTimeCost.setText(String.format(getString(R.string.timecost_value), result.getTimeCost()));

                String clientId = MqttClient.generateClientId();
                client =
                        new MqttAndroidClient(classify.this, "tcp://broker.hivemq.com:1883",
                                clientId);

                try {
                    IMqttToken token = client.connect();
                    token.setActionCallback(new IMqttActionListener() {
                        @Override
                        public void onSuccess(IMqttToken asyncActionToken) {
                            // We are connected
                            Toast.makeText(classify.this, "Commected", Toast.LENGTH_SHORT);
                            String topic = "iot_project_trash/bin";
                            String payload;
                            if(organic == true)
                                payload = "organic";
                            else
                                payload = "recycle";
                            byte[] encodedPayload = new byte[0];
                            try {
                                encodedPayload = payload.getBytes("UTF-8");
                                MqttMessage message = new MqttMessage(encodedPayload);
                                client.publish(topic, message);
                            } catch (UnsupportedEncodingException | MqttException e) {
                                e.printStackTrace();
                            }
                        }

                        @Override
                        public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                            // Something went wrong e.g. connection timeout or firewall problems
                            Toast.makeText(classify.this, "something wrong", Toast.LENGTH_SHORT);


                        }
                    });
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }
        });
        Button back_button = (Button)findViewById(R.id.btn_clear);
        back_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(classify.this, MainActivity.class);
                startActivity(i);
            }
        });
    }

    @OnClick(R.id.buttonCamera)
    void onCameraClick() {
        openCameraIntent();
    }

    @OnClick(R.id.btn_detect)
    void onDetectClick() {
            if (mClassifier == null) {
                return;
            }

            Bitmap image = Bitmap.createScaledBitmap(bitmap, 299, 299, true);
            Result result = mClassifier.classify(image);
            renderResult(result);
    }

    @OnClick(R.id.btn_clear)
    void onClearClick() {
        selected_image.setImageBitmap(null);
    }

    private void init() {
        try {
            mClassifier = new Classifier(this);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void renderResult(Result result) {
        mTvPrediction.setText(String.valueOf(result.getNumber()));
        mTvProbability.setText(String.valueOf(result.getProbability()));
        mTvTimeCost.setText(String.format(getString(R.string.timecost_value),
                result.getTimeCost()));
    }

    private void openCameraIntent() {
        ContentValues values = new ContentValues();
        values.put(MediaStore.Images.Media.TITLE, "New Picture");
        values.put(MediaStore.Images.Media.DESCRIPTION, "From your Camera");
        // tell camera where to store the resulting picture
        imageUri = getContentResolver().insert(
                MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        // start camera, and wait for it to finish
        startActivityForResult(intent, REQUEST_IMAGE);
    }
}
